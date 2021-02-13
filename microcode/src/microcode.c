#include "arguments.h"
#include "lexer.h"
#include "parser.h"
#include "output.h"

int main(int arg_len, char **arg) {
	if (!readArguments(arg, (size_t)arg_len)) return 1;

	size_t token_len;
	struct Token *token = buildTokens(&token_len);

	uint8_t *output = parseTokens(token, token_len);

	outputToFile(output, OUTPUT_SIZE);
	return 0;
}