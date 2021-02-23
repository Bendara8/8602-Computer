#include "token.h"
#include <stdlib.h>
#include <stdio.h>

static struct {
	enum TokenType;
	char chr;
} TOK_CHR_TBL[] = {
	{TOK_NEWLINE, '\n'},
	{TOK_COMMA,   ','},
	{TOK_HASH,    '#'},
	{TOK_PLUS,    '+'},
	{TOK_MINUS,   '-'},
	{TOK_EQUALS,  '='},
	{TOK_LBRACE,  '{'},
	{TOK_RBRACE,  '}'},
	{TOK_DOT,     '.'},
	// registers
	{TOK_A, 'A'},
	{TOK_F, 'F'},
	{TOK_K, 'K'},
	{TOK_P, 'P'},
	{TOK_B, 'B'},
	{TOK_S, 'S'},
	{TOK_I, 'I'},
};
static size_t TOK_CHR_LEN = sizeof TOK_CHR_TBL / sizeof TOK_CHR_TBL[0];

static struct {
	enum TokenType;
	char *str;
} TOK_STR_TBL[] = {
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

void initTokenQueue(void) {
	token = malloc(token_cap * sizeof token[0]);
	if (token == NULL) {
		puts("Unable to allocate token queue");
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
}

struct Token *newToken(void) {
	if (token_len == token_cap) {
		token_cap *= 2;
		token = realloc(token, token_cap * sizeof token[0]);
		if (token == NULL) {
			puts("Unable to allocate token queue");
			exit(EXIT_FAILURE);
		}
	}
	return &token[token_len++];
}