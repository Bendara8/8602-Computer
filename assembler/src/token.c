#include "token.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

static bool strEqualNoCase(char *candidate, char *test);

static struct {
	enum TokenType type;
	char chr;
} TOK_CHR_TBL[] = {
	{TOK_NEWLINE, '\n'},
	{TOK_HASH,    '#'},
	{TOK_PLUS,    '+'},
	{TOK_MINUS,   '-'},
	{TOK_EQUALS,  '='},
	{TOK_LBRACE,  '{'},
	{TOK_RBRACE,  '}'},
};
static size_t TOK_CHR_LEN = sizeof TOK_CHR_TBL / sizeof TOK_CHR_TBL[0];

static struct {
	enum TokenType type;
	char *str;
} TOK_STR_TBL[] = {
	// registers
	{TOK_A, "A"},
	{TOK_F, "F"},
	{TOK_K, "K"},
	{TOK_P, "P"},
	{TOK_B, "B"},
	{TOK_S, "S"},
	{TOK_I, "I"},
	// directives
	{TOK_ORIGIN, "ORIGIN"},
	{TOK_EXPORT, "EXPORT"},
	{TOK_BYTE,   "BYTE"},
	{TOK_WORD,   "WORD"},
	{TOK_LONG,   "LONG"},
	{TOK_QUAD,   "QUAD"},
	{TOK_STRZ,   "STRZ"},
	// instructions
	{TOK_LOD, "LOD"}, {TOK_LDW, "LDW"}, {TOK_LEA, "LEA"},
	{TOK_STO, "STO"}, {TOK_STW, "STW"},
	{TOK_MOV, "MOV"},
	{TOK_PSH, "PSH"}, {TOK_PHW, "PHW"}, {TOK_PEA, "PEA"},
	{TOK_PUL, "PUL"}, {TOK_PLW, "PLW"},
	{TOK_COP, "COP"}, {TOK_CPW, "CPW"},
	{TOK_NOP, "NOP"},
	{TOK_NOT, "NOT"}, {TOK_AND, "AND"}, {TOK_ORA, "ORA"},
	{TOK_ADD, "ADD"}, {TOK_ADC, "ADC"},
	{TOK_SUB, "SUB"}, {TOK_SBC, "SBC"},
	{TOK_CMP, "CMP"},
	{TOK_ASR, "ASR"},
	{TOK_LSL, "LSL"}, {TOK_LSR, "LSR"},
	{TOK_ROL, "ROL"}, {TOK_ROR, "ROR"},
	{TOK_INC, "INC"}, {TOK_DEC, "DEC"},
	{TOK_ICT, "ICT"}, {TOK_DCT, "DCT"},
	{TOK_SET, "SET"}, {TOK_CLR, "CLR"},
	{TOK_BRZ, "BRZ"}, {TOK_BNZ, "BNZ"},
	{TOK_BRC, "BRC"}, {TOK_BNC, "BNC"},
	{TOK_BRN, "BRN"}, {TOK_BNN, "BNN"},
	{TOK_BRG, "BRG"}, {TOK_BNG, "BNG"},
	{TOK_BRA, "BRA"},
	{TOK_VEC, "VEC"},
	{TOK_JMP, "JMP"}, {TOK_JPL, "JPL"},
	{TOK_ENT, "ENT"}, {TOK_EXT, "EXT"},
	{TOK_JSR, "JSR"}, {TOK_JSL, "JSL"},
	{TOK_RTS, "RTS"}, {TOK_RTL, "RTL"}, {TOK_RTI, "RTI"},
	{TOK_BRK, "BRK"},
};
static size_t TOK_STR_LEN = sizeof TOK_STR_TBL / sizeof TOK_STR_TBL[0];

static struct Token *token = NULL;
static size_t token_len = 0, token_cap = 256;
static size_t token_pos = 0;

void initTokenQueue(void) {
	token = malloc(token_cap * sizeof token[0]);
	if (token == NULL) {
		puts("Unable to allocate token queue.");
		exit(EXIT_FAILURE);
	}
}

void deinitTokenQueue(void) {
	resetTokenQueue();
	free(token);
}

void resetTokenQueue(void) {
	for (size_t i = 0; i < token_len; ++i) {
		free(token[i].str);
	}
	token_len = 0;
	token_pos = 0;
}

struct Token *newToken(void) {
	if (token_len == token_cap) {
		token_cap *= 2;
		token = realloc(token, token_cap * sizeof token[0]);
		if (token == NULL) {
			puts("Unable to reallocate token queue.");
			exit(EXIT_FAILURE);
		}
	}
	return &token[token_len++];
}

struct Token *nextToken(void) {
	if (token_pos == token_len) return NULL;
	return &token[token_pos++];
}

struct Token *peekToken(void) {
	if (token_pos == token_len) return NULL;
	return &token[token_pos];
}

void seekToken(struct Token *target) {
	token_pos = (target >= token) ? target - token : 0;
}

enum TokenType charToToken(char chr) {
	for (size_t i = 0; i < TOK_CHR_LEN; ++i) {
		if (chr == TOK_CHR_TBL[i].chr) return TOK_CHR_TBL[i].type;
	}
	return TOK_NONE;
}

char tokenToChar(enum TokenType type) {
	for (size_t i = 0; i < TOK_CHR_LEN; ++i) {
		if (type == TOK_CHR_TBL[i].type) return TOK_CHR_TBL[i].chr;
	}
	return '\0';
}

enum TokenType stringToToken(char *str) {
	for (size_t i = 0; i < TOK_STR_LEN; ++i) {
		if (strEqualNoCase(str, TOK_STR_TBL[i].str)) return TOK_STR_TBL[i].type;
	}
	return TOK_NONE;
}

char *tokenToString(enum TokenType type) {
	for (size_t i = 0; i < TOK_STR_LEN; ++i) {
		if (type == TOK_STR_TBL[i].type) return TOK_STR_TBL[i].str;
	}
	return NULL;
}

bool strEqualNoCase(char *candidate, char *test) {
	size_t i;
	for (i = 0; candidate[i] != '\0' && test[i] != '\0'; ++i) {
		if (toupper(candidate[i]) != test[i]) return false;
	}
	if (candidate[i] == '\0' && test[i] == '\0') return true;
	return false;
}