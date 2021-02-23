#include "arguments.h"
#include "lexer.h"
#include "matcher.h"
#include "parser.h"
#include "linker.h"
#include "token.h"
#include <stdlib.h>
#include <string.h>

int main(int arg_len, char **arg) {
	initArguments();
	atexit(&deinitArguments);

	readArguments(arg, (size_t)arg_len);

	initTokenQueue();
	atexit(&deinitTokenQueue);

	size_t input_len = getInputLen();
	for (size_t i = 0; i < input_len; ++i) {
		lexFileIntoTokens(getInput(i));
		matchTokensIntoPatterns();
		parsePatternsIntoObject();
	}
	linkObjectsIntoBinary(getOutput());

	return EXIT_SUCCESS;
}