#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "interface.h"
#include "error.h"

static const char *CMD_STR[] = {
	"",
	"nano",
	"clock",
	"step",
	"run",
	"quit",
	"updates",
	"chip",
	"bus",
	"net",
	"hex",
	"set",
	"touch",
};
static const size_t CMD_STR_LEN = sizeof CMD_STR / sizeof CMD_STR[0];

void printInfo(void) {
	printf("--- 8602 Simulator ---\n");
}

static void splitInput(char *input_str, char **command, size_t command_len) {
	size_t i = 0, j = 0;
	char prev = ' ';
	while (input_str[i] != '\0') {
		if (isspace(prev) && !isspace(input_str[i])) {
			if (j < command_len) {
				command[j] = &input_str[i];
				++j;
			}
			else {
				raiseError(ERROR_LONG_COMMAND, command_len);
				j = 0;
				break;
			}
			prev = input_str[i];
		}
		else if (!isspace(prev) && isspace(input_str[i])) {
			prev = input_str[i];
			input_str[i] = '\0';
		}
		else prev = input_str[i];
		++i;
	}
	for (i = j; i < command_len; ++i) {
		command[i] = NULL;
	}
}

static enum Command strToCommand(char *str) {
	for (size_t i = 0; i < CMD_STR_LEN; ++i) {
		if (strcmp(str, CMD_STR[i]) == 0) return (enum Command)i;
	}
	return CMD_NONE;
}

static void runCommandNano(char **command, struct Circuit *circ) {
	command = command; circ = circ;
}

static void runCommandClock(char **command, struct Circuit *circ) {
	command = command; circ = circ;
}

static void runCommandStep(char **command, struct Circuit *circ) {
	command = command; circ = circ;
}

static void runCommandRun(char **command, struct Circuit *circ) {
	command = command; circ = circ;
}

static void runCommandUpdates(char **command, struct Circuit *circ) {
	command = command; circ = circ;
}

static void runCommandChip(char **command, struct Circuit *circ) {
	command = command; circ = circ;
}

static void runCommandBus(char **command, struct Circuit *circ) {
	command = command; circ = circ;
}

static void runCommandNet(char **command, struct Circuit *circ) {
	command = command; circ = circ;
}

static void runCommandHex(char **command, struct Circuit *circ) {
	command = command; circ = circ;
}

static void runCommandSet(char **command, struct Circuit *circ) {
	command = command; circ = circ;
}

static void runCommandTouch(char **command, struct Circuit *circ) {
	command = command; circ = circ;
}

void enterInterface(struct Circuit *circ) {
	char input_str[64];
	char *command[8];
	setErrorLine(0);
	while (1) {
		printf("> ");
		fgets(input_str, sizeof input_str, stdin);
		input_str[strcspn(input_str, "\n")] = '\0';
		splitInput(input_str, command, sizeof command / sizeof command[0]);
		if (command[0]) switch (strToCommand(command[0])) {
			case CMD_NONE:    raiseError(ERROR_NOT_COMMAND, command[0]); break;
			case CMD_NANO:    runCommandNano(command, circ); break;
			case CMD_CLOCK:   runCommandClock(command, circ); break;
			case CMD_STEP:    runCommandStep(command, circ); break;
			case CMD_RUN:     runCommandRun(command, circ); break;
			case CMD_QUIT:    return;
			case CMD_UPDATES: runCommandUpdates(command, circ); break;
			case CMD_CHIP:    runCommandChip(command, circ); break;
			case CMD_BUS:     runCommandBus(command, circ); break;
			case CMD_NET:     runCommandNet(command, circ); break;
			case CMD_HEX:     runCommandHex(command, circ); break;
			case CMD_SET:     runCommandSet(command, circ); break;
			case CMD_TOUCH:   runCommandTouch(command, circ); break;
		}
	}
}