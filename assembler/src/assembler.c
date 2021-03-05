#include "arguments.h"
#include "lexer.h"
#include "matcher.h"
#include "parser.h"
#include "linker.h"
#include "token.h"
#include "pattern.h"
#include <stdlib.h>

int main(int arg_len, char **arg) {
	initArguments();
	atexit(&deinitArguments);

	readArguments(arg, (size_t)arg_len);

	initTokenQueue();
	atexit(&deinitTokenQueue);

	initPatternQueue();
	atexit(&deinitPatternQueue);

	size_t input_len = getInputLen();
	for (size_t i = 0; i < input_len; ++i) {
		char *input = getInput(i);
		lexFileIntoTokens(input);
		matchTokensIntoPatterns(input);
		parsePatternsIntoObject(input);
	}
	linkObjectsIntoBinary(getOutput());

	return EXIT_SUCCESS;
}