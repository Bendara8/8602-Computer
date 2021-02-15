#include "arguments.h"
#include "interface.h"
#include "command.h"
#include "display.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum Option {
	OPT_NONE,
	OPT_HELP,
	OPT_FLASH,
	OPT_MICROCODE,
	OPT_DIMENSION,
	OPT_SCALE,
	OPT_RUN,
};

static const struct {
	enum Option option;
	char *single, *word;
} OPTION_TABLE[] = {
	{OPT_HELP,      "-h", "--help"},
	{OPT_FLASH,     "-f", "--flash"},
	{OPT_MICROCODE, "-m", "--microcode"},
	{OPT_DIMENSION, "-d", "--dimension"},
	{OPT_SCALE,     "-s", "--scale"},
	{OPT_RUN,       "-r", "--run"},
};
static const size_t OPTION_TABLE_LEN = sizeof OPTION_TABLE / sizeof OPTION_TABLE[0];

static const char *HELP_MSG =
	"---8602 Emulator Invocation Options---\n"
	"Syntax:                          | Description:\n"
	"-h, --help                       | Display this message.\n"
	"-f, --flash <path>               | Override the default location of the file with flash\n"
	"                                 | memory data. Default location is 'res/flash.bin'.\n"
	"-m, --microcode <path>           | Override the default location of the file with compiled\n"
	"                                 | microcode. Default location is 'res/microcode.bin'.\n"
	"-d, --dimension <width> <height> | Override the default display dimensions.\n"
	"                                 | Default values are 512 by 256. Values must\n"
	"                                 | be between 128 by 64 and 512 by 256.\n"
	"-s, --scale <value>              | Override the default display scale. Default value is 1.\n"
	"                                 | Value must be 1, 2, or 3.\n"
	"-r, --run                        | Run the emulator immediately after invocation.";

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

			case OPT_DIMENSION:
				if (!assertNextArgument(i, arg, arg_len)) return false;
				uint16_t width = strtoul(arg[++i], NULL, 10);
				if (!assertNextArgument(i, arg, arg_len)) return false;
				setDimensions(width, strtoul(arg[++i], NULL, 10));
				break;

			case OPT_SCALE:
				if (!assertNextArgument(i, arg, arg_len)) return false;
				setScale(strtoul(arg[++i], NULL, 10));
				break;

			case OPT_RUN:
				setRunning(true);
				setFeedback("Running...");
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
