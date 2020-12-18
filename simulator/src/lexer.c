#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "token.h"
#include "error.h"

static void discardLine(FILE *file) {
	char next;
	do next = getc(file); while (next != '\n' && next != EOF);
}

static unsigned parseNum(char *lexeme) {
	unsigned result = 0;
	size_t i = 0;
	while (lexeme[i] != '\0') {
		if (!isdigit(lexeme[i])) {
			raiseError(
				ERROR_NOT_NUM,
				lexeme
			);
			return 0;
		}
		result = result * 10 + (lexeme[i++] & 0x0F);
	}
	return result;
}

static void tokenizeLexeme(char *lexeme, struct Token *tok) {
	if (isdigit(lexeme[0])) {
		tok->type = TOK_NUM;
		tok->num = parseNum(lexeme);
	}
	else if (strcmp(lexeme, "bus") == 0) {
		tok->type = TOK_BUS;
	}
	else if (strcmp(lexeme, "net") == 0) {
		tok->type = TOK_NET;
	}
	else if (strcmp(lexeme, "chip") == 0) {
		tok->type = TOK_CHIP;
	}
	else if (strcmp(lexeme, "in") == 0) {
		tok->type = TOK_IN;
	}
	else if (strcmp(lexeme, "out") == 0) {
		tok->type = TOK_OUT;
	}
	else if (strcmp(lexeme, "null") == 0) {
		tok->type = TOK_NULL;
	}
	else {
		tok->type = TOK_SYMBOL;
		tok->str = malloc(strlen(lexeme) + 1);
		if (!tok->str) raiseAbort(
			ABORT_ALLOCATION
		);
		addFreeTarget(tok->str);
		strcpy(tok->str, lexeme);
	}
}

static void tokenizePunct(char chr, struct Token *tok) {
	switch (chr) {
		case '{': tok->type = TOK_BEG_BRACE; break;
		case '}': tok->type = TOK_END_BRACE; break;
		case '=': tok->type = TOK_EQUALS; break;
		case ':': tok->type = TOK_COLON; break;
		case ',': tok->type = TOK_COMMA; break;
		case '-': tok->type = TOK_DASH; break;
		default: break;
	}
}

static int isPunct(char chr) {
	return strchr("{}=:,-", chr) != NULL;
}

static int getLexeme(char *lexeme, size_t cap, FILE *file) {
	char next;
	for (size_t i = 0; i < cap; ++i) {
		next = getc(file);
		if (next == EOF || isspace(next) || isPunct(next)) {
			ungetc(next, file);
			lexeme[i] = '\0';
			return 1;
		}
		else lexeme[i] = next;
	}
	lexeme[cap - 1] = '\0';
	raiseError(
		ERROR_LONG_LEXEME,
		lexeme,
		cap
	);
	return 0;
}

void lexFile(char *path, struct TokenVec *tok_vec) {
	// open file
	FILE *file = fopen(path, "r");
	if (!file) raiseAbort(
		ABORT_NO_FILE,
		path
	);

	// generate token vector
	unsigned line = 1;
	setErrorPath(path);
	setErrorLine(line);
	struct Token *next_tok;
	char next_chr;
	char lexeme[32];
	while (1) {
		next_chr = getc(file);
		if (next_chr == EOF) break;
		else if (next_chr == '\n') {
			++line;
			setErrorLine(line);
		}
		else if (next_chr == '#') {
			discardLine(file);
			++line;
			setErrorLine(line);
		}
		else if (!isspace(next_chr)) {
			next_tok = newToken(tok_vec);
			next_tok->line = line;
			if (isPunct(next_chr)) {
				tokenizePunct(next_chr, next_tok);
			}
			else {
				ungetc(next_chr, file);
				if (!getLexeme(lexeme, sizeof lexeme, file)) {
					discardLine(file);
					++line;
					setErrorLine(line);
				}
				else tokenizeLexeme(lexeme, next_tok);
			}
		}
	}
	fclose(file);
}