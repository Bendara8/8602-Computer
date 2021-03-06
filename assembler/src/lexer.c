#include "lexer.h"
#include "token.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

static void lexDefault(int chr);
static void lexDecimal(void);
static void lexHex(void);
static void lexBinary(void);
static void lexString(void);
static char *getLexeme(void);
static void discardLine(void);
static void printError(void);

static char *path = NULL;
static FILE *file = NULL;
static uint32_t line = 1;
static uint32_t err_ct = 0;

void lexFileIntoTokens(char *path_) {
	path = path_;
	file = fopen(path, "r");
	if (file == NULL) {
		printf("Unable to open target file '%s'.\n", path);
		exit(EXIT_FAILURE);
	}

	resetTokenQueue();

	int chr = getc(file);
	while (chr != EOF) {
		switch (chr) {
			case ';':  discardLine(); break;
			case '$':  lexHex(); break;
			case '%':  lexBinary(); break;
			case '"':  lexString(); break;
			default:
				if (isdigit(chr)) {
					ungetc(chr, file);
					lexDecimal();
				}
				else lexDefault(chr);
		}
		chr = getc(file);
	}
	struct Token *token = newToken();
	token->type = TOK_NEWLINE;
	token->line = line;
	token->str = NULL;
	fclose(file);
	if (err_ct > 0) {
		printf("(%s) Had %u errors.\n", path, err_ct);
		exit(EXIT_FAILURE);
	}
}

void lexDefault(int chr) {
	enum TokenType type = charToToken((char)chr);
	if (type != TOK_NONE) {
		struct Token *token = newToken();
		token->type = type;
		token->line = line;
		token->str = NULL;
		if (type == TOK_NEWLINE) ++line;
	}
	else if (isalpha(chr) || chr == '_') {
		ungetc(chr, file);
		char *str = getLexeme();
		type = stringToToken(str);
		if (type == TOK_NONE) type = TOK_NAME;
		struct Token *token = newToken();
		token->type = type;
		token->line = line;
		token->str = str;
	}
	else if (!isspace(chr)) {
		printError();
		printf("Invalid character '%c'.\n", (char)chr);
	}
}

void lexDecimal(void) {
	char *str = getLexeme();
	for (size_t i = 0; str[i] != '\0'; ++i) {
		if (!isdigit(str[i]) && str[i] != '_') {
			printError();
			printf("Invalid character '%c' in decimal number.\n", str[i]);
			return;
		}
	}
	struct Token *token = newToken();
	token->type = TOK_DECIMAL;
	token->line = line;
	token->str = str;
}

void lexHex(void) {
	char *str = getLexeme();
	for (size_t i = 0; str[i] != '\0'; ++i) {
		if (!isxdigit(str[i]) && str[i] != '_') {
			printError();
			printf("Invalid character '%c' in hexadecimal number.\n", str[i]);
			return;
		}
	}
	struct Token *token = newToken();
	token->type = TOK_HEX;
	token->line = line;
	token->str = str;
}

void lexBinary(void) {
	char *str = getLexeme();
	for (size_t i = 0; str[i] != '\0'; ++i) {
		if (str[i] != '0' && str[i] != '1' && str[i] != '_') {
			printError();
			printf("Invalid character '%c' in binary number.\n", str[i]);
			return;
		}
	}
	struct Token *token = newToken();
	token->type = TOK_BINARY;
	token->line = line;
	token->str = str;
}

void lexString(void) {
	size_t str_len = 0, str_cap = 32;
	char *str = malloc(str_cap * sizeof str[0]);
	if (str == NULL) {
		puts("Unable to allocate string.");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	bool escape = false;
	int chr = getc(file);
	while (chr != '"' && !escape) {
		if (chr == '\n' || chr == EOF) {
			ungetc(chr, file);
			printError();
			puts("Unexpected end of line.");
			return;
		}
		escape = (chr == '\\');
		if (str_len == str_cap - 1) {
			str_cap *= 2;
			str = realloc(str, str_cap * sizeof str[0]);
			if (str == NULL) {
				puts("Unable to reallocate string.");
				fclose(file);
				exit(EXIT_FAILURE);
			}
		}
		str[str_len++] = (char)chr;
	}
	str[str_len] = '\0';
	struct Token *token = newToken();
	token->type = TOK_STRING;
	token->line = line;
	token->str = str;
}

char *getLexeme(void) {
	size_t lexeme_len = 0, lexeme_cap = 16;
	char *lexeme = malloc(lexeme_cap * sizeof lexeme[0]);
	if (lexeme == NULL) {
		puts("Unable to allocate lexeme.");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	int chr = getc(file);
	while (isalnum(chr) || chr == '_') {
		if (lexeme_len == lexeme_cap - 1) {
			lexeme_cap *= 2;
			lexeme = realloc(lexeme, lexeme_cap * sizeof lexeme[0]);
			if (lexeme == NULL) {
				puts("Unable to reallocate lexeme.");
				fclose(file);
				exit(EXIT_FAILURE);
			}
		}
		lexeme[lexeme_len++] = (char)chr;
		chr = getc(file);
	}
	ungetc(chr, file);
	lexeme[lexeme_len] = '\0';
	return lexeme;
}

void discardLine(void) {
	int chr = getc(file);
	while (chr != '\n') {
		if (chr == EOF) return;
		chr = getc(file);
	}
	ungetc(chr, file);
}

void printError(void) {
	printf("(%s:%u) ", path, line);
	discardLine();
	++err_ct;
}