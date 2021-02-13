#ifndef HDR_PARSER
#define HDR_PARSER
#include "lexer.h"

#define OUTPUT_SIZE (3 * 524288)

uint8_t *parseTokens(
	struct Token *token,
	size_t token_len
);

#endif