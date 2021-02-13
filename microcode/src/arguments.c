#include "arguments.h"
#include <stdio.h>
#include <string.h>

enum Option {
	OPT_NONE,
	OPT_HELP,
	OPT_OUTPUT,
};

static struct {
	enum Option const option;
	char const *const single, *word;
} const OPTION_TABLE[] = {
	{OPT_HELP,   "-h", "--help"},
	{OPT_OUTPUT, "-o", "--output"},
};
static size_t const OPTION_TABLE_LEN = sizeof OPTION_TABLE / sizeof OPTION_TABLE[0];

static char const *const HELP_MSG =
	"---8602 Microcode Compiler Invocation Options---\n"
	"Syntax              | Description\n"
	"-h, --help          | Display this message\n"
	"-o, --output <path> | Set output target, include a '#' in the path to split output into 3 files";

static char *const DEFAULT_TARGET = "microcode.src";
static char *const DEFAULT_OUTPUT = "microcode.bin";

static char *target_path = DEFAULT_TARGET, *output_path = DEFAULT_OUTPUT;

static enum Option searchOptionTable(char *string);
static bool assertNextArgument(size_t i, char **arg, size_t arg_len);

bool readArguments(char **arg, size_t arg_len) {
	for (size_t i = 1; i < arg_len; ++i) {
		if (arg[i][0] == '-') {
			if (target_path == DEFAULT_TARGET) switch (searchOptionTable(arg[i])) {
				case OPT_NONE:
					printf("'%s' is not an option\n", arg[i]);
					return false;

				case OPT_HELP:
					puts(HELP_MSG);
					return false;

				case OPT_OUTPUT:
					if (!assertNextArgument(i, arg, arg_len)) return false;
					if (output_path == DEFAULT_OUTPUT) output_path = arg[++i];
					else {
						puts("Too many output targets");
						return false;
					}
					break;
			}
			else {
				puts("Options given after target");
				return false;
			}
		}
		else {
			if (target_path == DEFAULT_TARGET) {
				target_path = arg[i];
			}
			else {
				puts("Too many targets");
				return false;
			} 
		}
	}
	return true;
}

enum Option searchOptionTable(char *string) {
	for (size_t i = 0; i < OPTION_TABLE_LEN; ++i) {
		if (
			strcmp(OPTION_TABLE[i].single, string) == 0 ||
			strcmp(OPTION_TABLE[i].word, string) == 0
		) return OPTION_TABLE[i].option;
	}
	return OPT_NONE;
}

bool assertNextArgument(size_t i, char **arg, size_t arg_len) {
	if (i + 1 >= arg_len) {
		printf("Expected argument after '%s'\n", arg[i]);
		return false;
	}
	return true;
}

char *getTargetPath(void) {
	return target_path;
}

char *getOutputPath(void) {
	return output_path;
}