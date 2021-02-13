#include "lexer.h"
#include "arguments.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define NAME_CAP 32

static size_t const INIT_TOKEN_CAP = 256;

static struct Token *token = NULL;
static size_t token_len = 0, token_cap = INIT_TOKEN_CAP;
static uint32_t line = 1, col = 1;
static uint32_t error_count = 0;

static void printError(FILE *file);
static void unexpectedEOF(void);
static void discardLine(FILE *file);

static void addOpcode(FILE *file);
static void addName(FILE *file);
static void addFlag(FILE *file);
static void addSymbol(FILE *file);
static void addToken(enum TokenType type, FILE *file);

struct Token *buildTokens(size_t *token_len_ptr) {
	FILE *file = fopen(getTargetPath(), "r");
	if (!file) {
		printf("Unable to open target file '%s'\n", getTargetPath());
		exit(1);
	}

	token = malloc(token_cap * sizeof token[0]);
	if (!token) {
		puts("Unable to allocate token array");
		fclose(file);
		exit(1);
	}
	token_len = 0;

	int ch = getc(file);
	while (ch != EOF) {
		switch (ch) {
			case ';':  discardLine(file); break;
			case '$':  addOpcode(file); break;
			case '"':  addName(file); break;
			case '!':  addFlag(file); break;
			case '*':  addToken(TOK_STAR, file); break;
			case '~':  addToken(TOK_TILDE, file); break;
			case '\n':
				++line;
				col = 1;
				break;

			default:
				if (isupper(ch) || ch == '-') {
					ungetc(ch, file);
					--col;
					addSymbol(file);
				}
				else if (!isspace(ch)) {
					printError(file);
					printf("Unexpected character '%c'\n", (char)ch);
				}
				break;
		}
		ch = getc(file);
		++col;
	}

	fclose(file);
	*token_len_ptr = token_len;
	if (error_count > 0) {
		freeTokenArr(token, token_len);
		printf("%s: Had %u errors\n", getTargetPath(), error_count);
		exit(1);
	}
	return token;
}

void freeTokenArr(struct Token *token, size_t token_len) {
	for (size_t i = 0; i < token_len; ++i) {
		if (token[i].type == TOK_NAME) free(token[i].name);
	}
	free(token);
}

void printError(FILE *file) {
	printf("%s:%u:%u: ", getTargetPath(), line, col);
	discardLine(file);
	++error_count;
}

void unexpectedEOF(void) {
	printf("%s: Unexpected end of file", getTargetPath());
	++error_count;
}

void unexpectedEOL(void) {
	printf("%s:%u:%u: Unexpected end of line", getTargetPath(), line, col);
	++error_count;
	++line;
	col = 1;
}

void discardLine(FILE *file) {
	int ch = getc(file);
	while (ch != '\n') {
		if (ch == EOF) return;
		ch = getc(file);
	}
	++line;
	col = 1;
}

void addOpcode(FILE *file) {
	uint8_t opcode = 0;
	int ch;
	for (size_t i = 0; i < 2; ++i) {
		ch = getc(file);
		++col;
		switch (ch) {
			case EOF:  unexpectedEOF(); return;
			case '\n': unexpectedEOL(); return;
			default: break;
		}
		uint8_t digit;
		if (isdigit(ch)) digit = ch & 0x0F;
		else if (isxdigit(ch)) digit = toupper(ch) - 'A' + 10;
		else {
			printError(file);
			printf("'%c' is not a hexadecimal digit\n", (char)ch);
			return;
		}
		opcode <<= 4;
		opcode |= digit;
	}
	addToken(TOK_OPCODE, file);
	token[token_len - 1].opcode = opcode;
}

void addName(FILE *file) {
	char name[NAME_CAP];
	size_t name_len = 0;
	int ch = getc(file);
	++col;
	while (ch != '"') {
		switch (ch) {
			case EOF:  unexpectedEOF(); return;
			case '\n': unexpectedEOL(); return;
			default: break;
		}
		if (name_len == NAME_CAP - 1) {
			name[NAME_CAP - 1] = '\0';
			printError(file);
			printf("Name '%s' is too long (max %u)\n", name, NAME_CAP);
			return;
		}
		name[name_len++] = ch;
		ch = getc(file);
		++col;
	}
	name[name_len] = '\0';
	addToken(TOK_NAME, file);
	token[token_len - 1].name = malloc((name_len + 1) * sizeof token[token_len - 1].name[0]);
	if (!token[token_len - 1].name) {
		puts("Unable to allocate name");
		freeTokenArr(token, token_len);
		fclose(file);
		exit(1);
	}
	strcpy(token[token_len - 1].name, name);
}

void addFlag(FILE *file) {
	enum Flag flag = 0;
	uint8_t value = 0;
	int ch = getc(file);
	++col;
	switch (ch) {
		case EOF:  unexpectedEOF(); return;
		case '\n': unexpectedEOL(); return;
		case 'Z': flag = FLG_Z; break;
		case 'N': flag = FLG_N; break;
		case 'C': flag = FLG_C; break;
		case 'I': flag = FLG_I; break;
		default:
			printError(file);
			printf("'%c' is not a flag\n", (char)ch);
			return;
	}
	ch = getc(file);
	++col;
	switch (ch) {
		case EOF:  unexpectedEOF(); return;
		case '\n': unexpectedEOL(); return;
		case '0': value = 0; break;
		case '1': value = 1; break;
		default:
			printError(file);
			printf("'%c' is not a value\n", (char)ch);
			return;
	}
	addToken(TOK_FLAG, file);
	token[token_len - 1].flag = flag;
	token[token_len - 1].value = value;
}

void addSymbol(FILE *file) {
	char symbol[2];
	int ch;
	for (size_t i = 0; i < 2; ++i) {
		ch = getc(file);
		++col;
		switch (ch) {
			case EOF:  unexpectedEOF(); return;
			case '\n': unexpectedEOL(); return;
			case ' ':
				printError(file);
				printf("Expected 2 characters before a space\n");
				return;
			default: break;
		}
		symbol[i] = (char)ch;
	}
	addToken(TOK_SYMBOL, file);
	token[token_len - 1].symbol[0] = symbol[0];
	token[token_len - 1].symbol[1] = symbol[1];
}

void addToken(enum TokenType type, FILE *file) {
	if (token_len == token_cap) {
		token_cap *= 2;
		token = realloc(token, token_cap * sizeof token[0]);
		if (!token) {
			puts("Unable to reallocate token array");
			fclose(file);
			exit(1);
		}
	}
	token[token_len].type = type;
	token[token_len].line = line;
	token[token_len].col = col;
	++token_len;
}
