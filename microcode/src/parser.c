#include "parser.h"
#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>

static uint8_t *output;

uint8_t *parseTokens(struct Token *token, size_t token_len, size_t output_size) {
	output = malloc(output_size * sizeof output[0]);
	if (!output) {
		printf("Unable to allocate output buffer\n");
		freeTokenArr(token, token_len);
		exit(1);
	}


	return output;
	freeTokenArr(token, token_len);
}