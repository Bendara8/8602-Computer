#ifndef HDR_PARSER
#define HDR_PARSER
#include "lexer.h"

uint8_t *parseTokens(
	struct Token *token,
	size_t token_len,
	size_t output_size
);

#endif