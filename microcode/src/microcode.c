#include "arguments.h"
#include "lexer.h"
#include "parser.h"
#include "output.h"

static size_t const OUTPUT_SIZE = 3 * 524288;

int main(int arg_len, char **arg) {
	if (!readArguments(arg, (size_t)arg_len)) return 1;

	size_t token_len;
	struct Token *token = buildTokens(&token_len);

	uint8_t *output = parseTokens(token, token_len, OUTPUT_SIZE);

	outputToFile(output, OUTPUT_SIZE);
	return 0;
}