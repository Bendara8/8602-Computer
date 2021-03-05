#include "matcher.h"
#include "token.h"
#include "pattern.h"
#include <stdlib.h>
#include <stdio.h>

static enum PatternType matchPattern(void);
static void discardLine(void);

static char *path = NULL;
static uint32_t err_ct = 0;

void matchTokensIntoPatterns(char *path_) {
	path = path_;
	resetPatternQueue();
	while (peekToken() != NULL) {
		struct Token *curr_token = peekToken();
		if (curr_token->type == TOK_NEWLINE) {
			nextToken();
			continue;
		}
		enum PatternType type = matchPattern();
		if (type == PAT_NONE) {
			printf("(%s:%u) Invalid token ", path, curr_token->line);
			char chr = tokenToChar(curr_token->type);
			if (chr != '\0') {
				if (chr != '\n') printf("'%c'", chr);
				else printf("'\\n'");
			}
			else {
				char *str = curr_token->str;
				if (str != NULL) printf("'%s'", str);
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

// TODO add TOK_INS to args, then add TOK_INS-less patterns for defines
enum PatternType matchPattern(void) {
	static struct {
		enum PatternType type;
		enum TokenType token[12];
	} PAT_TOK_TBL[] = {
		// name
		{PAT_INS_IMMNAME,     {TOK_INS, TOK_HASH, TOK_NAME, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_IMMNAME_OFF, {TOK_INS, TOK_HASH, TOK_NAME, TOK_PLUS, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_NAME,     {TOK_INS, TOK_NAME, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_NAME_OFF, {TOK_INS, TOK_NAME, TOK_PLUS, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_P_NAME,     {TOK_INS, TOK_P, TOK_PLUS, TOK_NAME, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_P_NAME_OFF, {TOK_INS, TOK_P, TOK_PLUS, TOK_NAME, TOK_PLUS, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		// immediate
		{PAT_INS_IMM,     {TOK_INS, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_IMM_OFF, {TOK_INS, TOK_HASH, TOK_NUMBER, TOK_PLUS, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		// absolute
		{PAT_INS_ABS,     {TOK_INS, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_ABS_OFF, {TOK_INS, TOK_NUMBER, TOK_PLUS, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		// base relative
		{PAT_INS_B_IMM,     {TOK_INS, TOK_B, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_B_IMM_OFF, {TOK_INS, TOK_B, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_PLUS, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		// pointer immediate index
		{PAT_INS_P_IMM,     {TOK_INS, TOK_P, TOK_PLUS, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_P_IMM_OFF, {TOK_INS, TOK_P, TOK_PLUS, TOK_HASH, TOK_NUMBER, TOK_PLUS, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		// pointer absolute index
		{PAT_INS_P_ABS,     {TOK_INS, TOK_P, TOK_PLUS, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_P_ABS_OFF, {TOK_INS, TOK_P, TOK_PLUS, TOK_NUMBER, TOK_PLUS, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		// pointer base relative index
		{PAT_INS_P_B_IMM,     {TOK_INS, TOK_P, TOK_PLUS, TOK_B, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_P_B_IMM_OFF, {TOK_INS, TOK_P, TOK_PLUS, TOK_B, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_PLUS, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		// instruction relative
		{PAT_INS_I_IMM, {TOK_INS, TOK_I, TOK_ARITH, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		// register
		{PAT_INS_A_F, {TOK_INS, TOK_A, TOK_F, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_F_A, {TOK_INS, TOK_F, TOK_A, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_A_K, {TOK_INS, TOK_A, TOK_K, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_K_A, {TOK_INS, TOK_K, TOK_A, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_P_B, {TOK_INS, TOK_P, TOK_B, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_P_S, {TOK_INS, TOK_P, TOK_S, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_B_P, {TOK_INS, TOK_B, TOK_P, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_B_S, {TOK_INS, TOK_B, TOK_S, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_S_P, {TOK_INS, TOK_S, TOK_P, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_S_B, {TOK_INS, TOK_S, TOK_B, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_A, {TOK_INS, TOK_A, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_F, {TOK_INS, TOK_F, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_P, {TOK_INS, TOK_P, TOK_NEWLINE, TOK_NONE}},
		{PAT_INS_B, {TOK_INS, TOK_B, TOK_NEWLINE, TOK_NONE}},
		// no arg
		{PAT_INS, {TOK_INS, TOK_NEWLINE, TOK_NONE}},
		// labels
		{PAT_DEFINE,        {TOK_NAME, TOK_EQUALS, TOK_NONE}},
		{PAT_EXPORT_SCOPE, {TOK_EXPORT, TOK_NAME, TOK_LBRACE, TOK_NEWLINE, TOK_NONE}},
		{PAT_SCOPE,        {TOK_NAME, TOK_LBRACE, TOK_NEWLINE, TOK_NONE}},
		{PAT_ENDSCOPE,     {TOK_RBRACE, TOK_NEWLINE, TOK_NONE}},
		{PAT_EXPORT_LABEL, {TOK_EXPORT, TOK_NAME, TOK_NEWLINE, TOK_NONE}},
		{PAT_EXPORT_LABEL, {TOK_EXPORT, TOK_NAME, TOK_NONE}},
		{PAT_LABEL,        {TOK_NAME, TOK_NEWLINE, TOK_NONE}},
		{PAT_LABEL,        {TOK_NAME, TOK_NONE}},
		// directives
		{PAT_ORIGIN, {TOK_ORIGIN, TOK_HEX, TOK_NEWLINE, TOK_NONE}},
		{PAT_BYTE, {TOK_BYTE, TOK_NONE}},
		{PAT_WORD, {TOK_WORD, TOK_NONE}},
		{PAT_LONG, {TOK_LONG, TOK_NONE}},
		{PAT_QUAD, {TOK_QUAD, TOK_NONE}},
		{PAT_STRZ, {TOK_STRZ, TOK_STRING, TOK_NEWLINE, TOK_NONE}},
		// defines
		{PAT_NUMBER, {TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_B_PLUS_IMM, {TOK_B, TOK_PLUS, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_B_MINUS_IMM, {TOK_B, TOK_MINUS, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
		{PAT_P_PLUS_IMM, {TOK_P, TOK_PLUS, TOK_HASH, TOK_NUMBER, TOK_NEWLINE, TOK_NONE}},
	};
	static size_t PAT_TOK_LEN = sizeof PAT_TOK_TBL / sizeof PAT_TOK_TBL[0];
	struct Token *curr_token = peekToken();
	for (size_t i = 0; i < PAT_TOK_LEN; ++i) {
		for (size_t j = 0; true; ++j) {
			if (PAT_TOK_TBL[i].token[j] == TOK_NONE) return PAT_TOK_TBL[i].type;
			if (peekToken() == NULL) {
				printf("(%s) Unexpected end of tokens.\n", path);
				exit(EXIT_FAILURE);
			}
			if ((PAT_TOK_TBL[i].token[j] & ~TOK_GROUP) == 0x000) {
				if ((peekToken()->type & TOK_GROUP) != PAT_TOK_TBL[i].token[j]) break;
			}
			else if (peekToken()->type != PAT_TOK_TBL[i].token[j]) break;
			nextToken();
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