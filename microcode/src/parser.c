#include "parser.h"
#include "arguments.h"
#include "control.h"
#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct {
	enum TokenType type;
	char *string;
} const TYPE_STRING_TABLE[] = {
	{TOK_OPCODE,  "opcode"},
	{TOK_NAME,    "name"},
	{TOK_SYMBOL,  "symbol"},
	{TOK_FLAG,    "flag"},
	{TOK_STAR,  "star"},
	{TOK_TILDE,  "tilde"},
};
static size_t const TYPE_STRING_TABLE_LEN = sizeof TYPE_STRING_TABLE / sizeof TYPE_STRING_TABLE[0];

static size_t token_idx = 0;
static uint8_t *output = NULL;
static uint32_t error_count;

static void printError(struct Token *token, size_t token_len);
static void unexpectedToken(struct Token *token, size_t token_len, char *name);
static void unexpectedEndOfTokens(struct Token *token, size_t token_len);
static char *typeToStr(enum TokenType type);
static void discardSequence(struct Token *token, size_t token_len);

static bool assertNextTokenType(struct Token *token, size_t token_len, enum TokenType type, char *name);
static bool advanceTokenIdx(struct Token *token, size_t token_len);

static void parseOpcode(struct Token *token, size_t token_len);
static void parseAlternate(struct Token *token, size_t token_len);

static enum Control buildControlWord(struct Token *token, size_t token_len, char *name);
static bool isSymbolMatch(char const *a, char const *sb);

static void putSequence(
	struct Token *token,
	size_t token_len,
	bool reset,
	bool interrupt,
	uint8_t opcode,
	char *name
);
static void putControl(
	enum Control control,
	bool reset,
	bool interrupt,
	uint8_t opcode,
	uint8_t step,
	enum Flag flag,
	uint8_t value
);
static void putOpcode(
	enum Control control,
	uint8_t reset,
	uint8_t interrupt,
	uint8_t opcode,
	uint8_t step,
	enum Flag flag,
	uint8_t value
);
static void writeOutput(enum Control control, uint32_t address);

uint8_t *parseTokens(struct Token *token, size_t token_len) {
	output = malloc(OUTPUT_SIZE * sizeof output[0]);
	if (!output) {
		printf("Unable to allocate output buffer\n");
		freeTokenArr(token, token_len);
		exit(1);
	}
	memset(output, 0xFF, OUTPUT_SIZE);

	while (token_idx < token_len) {
		switch (token[token_idx].type) {
			case TOK_OPCODE: parseOpcode(token, token_len); break;
			case TOK_NAME:   parseAlternate(token, token_len); break;
			default:         unexpectedToken(token, token_len, NULL); break;
		}
	}

	freeTokenArr(token, token_len);
	if (error_count > 0) {
		printf("%s: Had %u errors\n", getTargetPath(), error_count);
		free(output);
		exit(1);
	}
	return output;
}

void printError(struct Token *token, size_t token_len) {
	printf("%s:%u:%u: ", getTargetPath(), token[token_idx].line, token[token_idx].col);
	++error_count;
}

void unexpectedToken(struct Token *token, size_t token_len, char *name) {
	printError(token, token_len);
	printf("Unexpected %s token", typeToStr(token[token_idx].type));
	if (name != NULL) printf(" in '%s'\n", name);
	else printf("\n");
	discardSequence(token, token_len);
}

void unexpectedEndOfTokens(struct Token *token, size_t token_len) {
	printf("%s: Unexpected end of tokens\n", getTargetPath());
	freeTokenArr(token, token_len);
	free(output);
	exit(1);
}

char *typeToStr(enum TokenType type) {
	for (size_t i = 0; i < TYPE_STRING_TABLE_LEN; ++i) {
		if (TYPE_STRING_TABLE[i].type == type) return TYPE_STRING_TABLE[i].string;
	}
	return "?";
}

void discardSequence(struct Token *token, size_t token_len) {
	while (token_idx < token_len && token[token_idx].type != TOK_OPCODE) ++token_idx;
}

bool assertNextTokenType(struct Token *token, size_t token_len, enum TokenType type, char *name) {
	if (!advanceTokenIdx(token, token_len)) unexpectedEndOfTokens(token, token_len);
	if (token[token_idx].type != type) {
		unexpectedToken(token, token_len, name);
		return false;
	}
	return true;
}

bool advanceTokenIdx(struct Token *token, size_t token_len) {
	++token_idx;
	return token_idx < token_len;
}

void parseOpcode(struct Token *token, size_t token_len) {
	uint8_t opcode = token[token_idx].opcode;
	if (!assertNextTokenType(token, token_len, TOK_NAME, NULL)) return;
	char *name = token[token_idx].name;
	putSequence(token, token_len, false, false, opcode, name);
}

void parseAlternate(struct Token *token, size_t token_len) {
	char *name = token[token_idx].name;
	if (strcmp(name, "reset") == 0) putSequence(token, token_len, true, false, 0, name);
	else if (strcmp(name, "interrupt") == 0) putSequence(token, token_len, false, true, 0, name);
	else unexpectedToken(token, token_len, name);
}

enum Control buildControlWord(struct Token *token, size_t token_len, char *name) {
	enum Control control = 0;
	for (size_t i = 0; i < 13; ++i) {
		if (!assertNextTokenType(token, token_len, TOK_SYMBOL, name)) return 0xFFFFFF;
		size_t j = 0;
		while (1) {
			if (CONTROL_TABLE[i][j].symbol[0] == '\0') {
				printError(token, token_len);
				printf("'%c%c' is an invalid symbol here\n", token[token_idx].symbol[0], token[token_idx].symbol[1]);
				discardSequence(token, token_len);
				return 0xFFFFFF;
			}
			if (isSymbolMatch(token[token_idx].symbol, CONTROL_TABLE[i][j].symbol)) {
				control |= CONTROL_TABLE[i][j].control;
				break;
			}
			++j;
		}
	}
	return control;
}

bool isSymbolMatch(char const *a, char const *b) {
	return (a[0] == b[0]) && (a[1] == b[1]);
}

void putSequence(
	struct Token *token,
	size_t token_len,
	bool reset,
	bool interrupt,
	uint8_t opcode,
	char *name
) {
	enum Control control = 0;
	uint8_t step = 0;
	while (1) {
		control = buildControlWord(token, token_len, name);
		if (control == 0xFFFFFF) return;
		if (!advanceTokenIdx(token, token_len)) {
			putControl(control, reset, interrupt, opcode, step, FLG_ALL, 0);
			return;
		}
		bool put_control = false;
		while (
			token[token_idx].type != TOK_SYMBOL &&
			token[token_idx].type != TOK_OPCODE &&
			token[token_idx].type != TOK_NAME
		) {
			switch (token[token_idx].type) {
				case TOK_STAR:  step = 0; break;
				case TOK_TILDE: --step; break;

				case TOK_FLAG:
					putControl(
						control, reset, interrupt, opcode, step,
						token[token_idx].flag,
						token[token_idx].value
					);
					put_control = true;
					break;

				default:
					unexpectedToken(token, token_len, name);
					return;
			}
			if (!advanceTokenIdx(token, token_len)) return;
		}
		if (!put_control) putControl(control, reset, interrupt, opcode, step, FLG_ALL, 0);
		if (token[token_idx].type == TOK_OPCODE || token[token_idx].type == TOK_NAME) return;
		--token_idx;
		++step;
	}
}

void putControl(
	enum Control control,
	bool reset,
	bool interrupt,
	uint8_t opcode,
	uint8_t step,
	enum Flag flag,
	uint8_t value
) {
	if (reset) {
		for (size_t i = 0; i < 4; ++i) {
			for (size_t j = 0; j < 256; ++j) {
				putOpcode(control, 1, i, j, step, flag, value);
			}
		}
	}
	else if (interrupt) {
		for (size_t i = 1; i < 4; ++i) {
			for (size_t j = 0; j < 256; ++j) {
				putOpcode(control, 0, i, j, step, flag, value);
			}
		}
	}
	else {
		for (size_t i = 0; i < 4; ++i) {
			putOpcode(control, 0, i, opcode, step, flag, value);
		}
	}
}

void putOpcode(
	enum Control control,
	uint8_t reset,
	uint8_t interrupt,
	uint8_t opcode,
	uint8_t step,
	enum Flag flag,
	uint8_t value
) {
	for (size_t i = 0; i < 16; ++i) {
		uint32_t address = 
			((reset     & 0x01) << 18) |
			((interrupt & 0x03) << 16) |
			((opcode    & 0xFF) << 8) |
			((i         & 0x0F) << 4) |
			((step      & 0x0F) << 0);
		if (flag == FLG_ALL) writeOutput(control, address);
		else if ((!!(flag & i)) == value) writeOutput(control, address);
	}
}

void writeOutput(enum Control control, uint32_t address) {
	control ^= CTRL_FI | CTRL_TO | CTRL_NA | CTRL_XZ | CTRL_EZ;
	output[(0 << 19) | address] = (control & 0x000000FF) >> 0;
	output[(1 << 19) | address] = (control & 0x0000FF00) >> 8;
	output[(2 << 19) | address] = (control & 0x00FF0000) >> 16;
}