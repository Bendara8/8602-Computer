#include "matcher.h"
#include "token.h"
#include "pattern.h"
#include <stdlib.h>
#include <stdio.h>

static enum PatternType matchPattern(void);
static void discardLine(void);

static char *path = NULL;
static uint32_t err_ct = 0;

void matchTokensIntoPatterns(char *p) {
	path = p;
	while (peekToken() != NULL) {
		struct Token *curr_token = peekToken();
		enum PatternType type = matchPattern();
		if (type == PAT_NONE) {
			printf("(%s:%u) Invalid token of type ", path, curr_token->line);
			char chr = tokenToChar(curr_token->type);
			if (chr != '\0') printf("%c", chr);
			else {
				char *str = tokenToString(curr_token->type);
				if (str != NULL) printf("%s", str);
				else printf("UNKNOWN");
			}
			printf(".\n");
			discardLine();
			++err_ct;
		}
		else {
			struct Pattern *pattern = newPattern();
			pattern->type = type;
			pattern->token = curr_token;
			if (
				type == PAT_BYTE ||
				type == PAT_WORD ||
				type == PAT_LONG ||
				type == PAT_QUAD ||
				type == PAT_STRZ
			) discardLine();
		}
	}
	if (err_ct > 0) {
		printf("(%s) Had %u errors.\n", path, err_ct);
		exit(EXIT_FAILURE);
	}
}

enum PatternType matchPattern(void) {
	static struct {
		enum PatternType type;
		enum TokenType token[11];
	} PAT_TOK_TBL[] = {
		// name
		{PAT_IMMNAME,     {TOK_HASH, TOK_NAME, TOK_NEWLINE, TOK_NONE}},
		{PAT_IMMNAME_OFF, {TOK_HASH, TOK_NAME, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_NAME,     {TOK_NAME, TOK_NEWLINE, TOK_NONE}},
		{PAT_NAME_OFF, {TOK_NAME, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_P_NAME,     {TOK_P, TOK_PLUS, TOK_NAME, TOK_NEWLINE, TOK_NONE}},
		{PAT_P_NAME_OFF, {TOK_P, TOK_PLUS, TOK_NAME, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		// immediate
		{PAT_IMM,     {TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_IMM_OFF, {TOK_HASH, TOK_NUMBER, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		// absolute
		{PAT_ABS,     {TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_ABS_OFF, {TOK_NUMBER, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		// base relative
		{PAT_B_IMM,     {TOK_B, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_B_IMM_OFF, {TOK_B, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		// pointer immediate index
		{PAT_P_IMM,     {TOK_P, TOK_PLUS, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_P_IMM_OFF, {TOK_P, TOK_PLUS, TOK_HASH, TOK_NUMBER, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		// pointer absolute index
		{PAT_P_ABS,     {TOK_P, TOK_PLUS, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_P_ABS_OFF, {TOK_P, TOK_PLUS, TOK_NUMBER, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		// pointer base relative index
		{PAT_P_B_IMM,     {TOK_P, TOK_PLUS, TOK_B, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_P_B_IMM_OFF, {TOK_P, TOK_PLUS, TOK_B, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		// instruction relative
		{PAT_I_IMM, {TOK_I, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		// register
		{PAT_A_F, {TOK_A, TOK_F, TOK_NEWLINE, TOK_NONE}},
		{PAT_F_A, {TOK_F, TOK_A, TOK_NEWLINE, TOK_NONE}},
		{PAT_A_K, {TOK_A, TOK_K, TOK_NEWLINE, TOK_NONE}},
		{PAT_K_A, {TOK_K, TOK_A, TOK_NEWLINE, TOK_NONE}},
		{PAT_P_B, {TOK_P, TOK_B, TOK_NEWLINE, TOK_NONE}},
		{PAT_P_S, {TOK_P, TOK_S, TOK_NEWLINE, TOK_NONE}},
		{PAT_B_P, {TOK_B, TOK_P, TOK_NEWLINE, TOK_NONE}},
		{PAT_B_S, {TOK_B, TOK_S, TOK_NEWLINE, TOK_NONE}},
		{PAT_S_P, {TOK_S, TOK_P, TOK_NEWLINE, TOK_NONE}},
		{PAT_S_B, {TOK_S, TOK_B, TOK_NEWLINE, TOK_NONE}},
		{PAT_A, {TOK_A, TOK_NEWLINE, TOK_NONE}},
		{PAT_F, {TOK_F, TOK_NEWLINE, TOK_NONE}},
		{PAT_P, {TOK_P, TOK_NEWLINE, TOK_NONE}},
		{PAT_B, {TOK_B, TOK_NEWLINE, TOK_NONE}},
		// labels
		{PAT_EXPORT_SCOPE, {TOK_EXPORT, TOK_NAME, TOK_LBRACE, TOK_NEWLINE, TOK_NONE}},
		{PAT_SCOPE,        {TOK_NAME, TOK_LBRACE, TOK_NEWLINE, TOK_NONE}},
		{PAT_ENDSCOPE,     {TOK_RBRACE, TOK_NEWLINE, TOK_NONE}},
		{PAT_EXPORT_LABEL, {TOK_EXPORT, TOK_NAME, TOK_NEWLINE, TOK_NONE}},
		{PAT_EXPORT_LABEL, {TOK_EXPORT, TOK_NAME, TOK_NONE}},
		{PAT_LABEL,        {TOK_NAME, TOK_NEWLINE, TOK_NONE}},
		{PAT_LABEL,        {TOK_NAME, TOK_NONE}},
		{PAT_EQUAL,        {TOK_NAME, TOK_EQUALS, TOK_NONE}},
		// directives
		{PAT_ORIGIN, {TOK_ORIGIN, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_BYTE, {TOK_BYTE, TOK_NONE}},
		{PAT_WORD, {TOK_WORD, TOK_NONE}},
		{PAT_LONG, {TOK_LONG, TOK_NONE}},
		{PAT_QUAD, {TOK_QUAD, TOK_NONE}},
		{PAT_STRZ, {TOK_STRZ, TOK_NONE}},
		// instructions
		{PAT_INS, {TOK_INS, TOK_NONE}},
	};
	static size_t PAT_TOK_LEN = sizeof PAT_TOK_TBL / sizeof PAT_TOK_TBL[0];
	struct Token *curr_token = peekToken();
	for (size_t i = 0; i < PAT_TOK_LEN; ++i) {
		for (size_t j = 0; true; ++j) {
			if (PAT_TOK_TBL[i].token[j] == TOK_NONE) return PAT_TOK_TBL[i].type;
			nextToken();
			if (peekToken() == NULL) {
				printf("(%s) Unexpected end of tokens.\n", path);
				exit(EXIT_FAILURE);
			}
			if ((peekToken()->type & TOK_GROUP) != (PAT_TOK_TBL[i].token[j] & TOK_GROUP)) break;
		}
		seekToken(curr_token);
	}
	nextToken();
	return PAT_NONE;
}

void discardLine(void) {
	while (peekToken() != NULL) {
		if (nextToken()->type == TOK_NEWLINE) return;
	}
	return;
}