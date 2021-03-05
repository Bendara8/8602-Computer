#include "arguments.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum Option {
	OPT_NONE,
	OPT_HELP,
	OPT_OUTPUT,
};

static enum Option searchOptionTable(char *arg);
static void assertNextArgument(size_t i, char **arg, size_t arg_len);

static struct {
	enum Option option;
	char *single, *word;
} OPT_TBL[] = {
	{OPT_HELP,   "-h", "--help"},
	{OPT_OUTPUT, "-o", "--output"},
};
static size_t OPT_TBL_LEN = sizeof OPT_TBL / sizeof OPT_TBL[0];

static char *HELP_MSG =
	"---8602 Assembler Invocation Options---\n"
	"Syntax:             | Description:\n"
	"-h, --help          | Displays this message.\n"
	"-o, --output <path> | Overrides default output path. Default path is 'out.bin'.";

static size_t input_len = 0, input_cap = 16;
static char **input = NULL;
static char *output = "out.bin";

void initArguments(void) {
	input = malloc(input_cap * sizeof input[0]);
	if (input == NULL) {
		puts("Unable to allocate input path array.");
		exit(EXIT_FAILURE);
	}
}

void deinitArguments(void) {
	free(input);
}

void readArguments(char **arg, size_t arg_len) {
	for (size_t i = 1; i < arg_len; ++i) {
		if (arg[i][0] == '-') {
			switch (searchOptionTable(arg[i])) {
				case OPT_HELP:
					puts(HELP_MSG);
					exit(EXIT_SUCCESS);
					break;

				case OPT_OUTPUT:
					assertNextArgument(i, arg, arg_len);
					output = arg[++i];
					break;

				case OPT_NONE:
					printf("'%s' is not an option\n", arg[i]);
					exit(EXIT_FAILURE);
					break;
			}
		}
		else {
			if (input_len == input_cap) {
				input_cap *= 2;
				input = realloc(input, input_cap * sizeof input[0]);
				if (input == NULL) {
					puts("Unable to reallocate input path array.");
					exit(EXIT_FAILURE);
				}
			}
			input[input_len++] = arg[i];
		}
	}
}

enum Option searchOptionTable(char *arg) {
	for (size_t i = 0; i < OPT_TBL_LEN; ++i) {
		if (
			strcmp(OPT_TBL[i].single, arg) == 0 ||
			strcmp(OPT_TBL[i].word, arg) == 0
		) return OPT_TBL[i].option;
	}
	return OPT_NONE;
}

void assertNextArgument(size_t i, char **arg, size_t arg_len) {
	if (i + 1 >= arg_len) {
		printf("Expected argument after '%s'.\n", arg[i]);
		exit(EXIT_FAILURE);
	}
}

size_t getInputLen(void) {
	return input_len;
}

char *getInput(size_t idx) {
	return input[idx];
}

char *getOutput(void) {
	return output;
}