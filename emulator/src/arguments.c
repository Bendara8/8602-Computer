#include "arguments.h"
#include "interface.h"
#include <stdio.h>
#include <string.h>

enum Option {
	OPT_NONE,
	OPT_HELP,
	OPT_FLASH,
	OPT_MICROCODE,
	OPT_RUN,
};

static const struct {
	enum Option option;
	char *single, *word;
} OPTION_TABLE[] = {
	{OPT_HELP,      "-h", "--help"},
	{OPT_FLASH,     "-f", "--flash"},
	{OPT_MICROCODE, "-m", "--microcode"},
	{OPT_RUN,       "-r", "--run"},
};
static const size_t OPTION_TABLE_LEN = sizeof OPTION_TABLE / sizeof OPTION_TABLE[0];

static const char *HELP_MSG =
	"---8602 Emulator Invocation Options---\n"
	"Syntax                 | Description\n"
	"-h, --help             | Display this message\n"
	"-f, --flash <path>     | Override the default location of the file with flash memory data\n"
	"                       | Default location is res/flash.bin\n"
	"-m, --microcode <path> | Override the default location of the file with compiled microcode\n"
	"                       | Default location is res/microcode.bin\n"
	"-r, --run              | Run the emulator immediately after invocation";

static char *flash_path = "res/flash.bin", *microcode_path = "res/microcode.bin";

static enum Option searchOptionTable(char *string);
static bool assertNextArgument(size_t i, char **arg, size_t arg_len);

bool readArguments(char **arg, size_t arg_len) {
	for (size_t i = 1; i < arg_len; ++i) {
		switch (searchOptionTable(arg[i])) {
			case OPT_NONE:
				printf("'%s' is not an option\n", arg[i]);
				return false;

			case OPT_HELP:
				puts(HELP_MSG);
				return false;

			case OPT_FLASH:
				if (!assertNextArgument(i, arg, arg_len)) return false;
				flash_path = arg[++i];
				break;

			case OPT_MICROCODE:
				if (!assertNextArgument(i, arg, arg_len)) return false;
				microcode_path = arg[++i];
				break;

			case OPT_RUN:
				setRunning(true);
				break;
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

char *getFlashPath(void) {
	return flash_path;
}

char *getMicrocodePath(void) {
	return microcode_path;
}
