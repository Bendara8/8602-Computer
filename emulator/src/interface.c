#include "interface.h"
#include "arguments.h"
#include "command.h"
#include "terminal.h"
#include "display.h"
#include "keyboard.h"
#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const size_t INIT_TOKEN_CAP = 8;
static const uint64_t CLOCK_PERIOD = 400; // clock period (ns)

enum Command {
	CMD_NONE,
	CMD_SIZE,
	CMD_SAVE,
	CMD_QUIT,
	CMD_RUN,
	CMD_HALT,
	CMD_RESET,
	CMD_MEM,
};

static bool isRunning = false;

static size_t tokenizeStr(char *str, char **token, size_t token_cap);
static enum Command strToCommand(char *str);

void enterInterface(void) {
	if (!initTerminal()) return;
	atexit(&deinitTerminal);

	while (1) {
		updateTerminal();
		if (isRunning) {
			clockSystem();
		}
		else {
			clockDisplay(0);
		}
	}
}

void clockSystem(void) {
	clockKeyboard(CLOCK_PERIOD);
	clockCPU();
	clockDisplay(CLOCK_PERIOD);
}

void stepInstruction(void) {
	do clockSystem(); while (getCPU()->step != 0);
}

void nextInstruction(void) {
	// step unless its a subroutine instruction then step and finish
}

void finishSubroutine(void) {
	// step until return
}

void runCommand(char *str) {
	size_t token_len, token_cap = INIT_TOKEN_CAP;
	char **token = malloc(token_cap * sizeof token[0]);
	if (!token) exit(1);
	token_len = tokenizeStr(str, token, token_cap);
	switch (strToCommand(token[0])) {
		case CMD_NONE: break;
		case CMD_SIZE:  commandSize(token, token_len);  break;
		case CMD_SAVE:  commandSave(token, token_len);  break;
		case CMD_QUIT:  free(token); exit(0);           break;
		case CMD_RUN:   commandRun(token, token_len);   break;
		case CMD_HALT:  commandHalt(token, token_len);  break;
		case CMD_RESET: commandReset(token, token_len); break;
		case CMD_MEM:   commandMem(token, token_len);   break;
	}
	free(token);
}

size_t tokenizeStr(char *str, char **token, size_t token_cap) {
	char prev = ' ';
	size_t token_len = 0;
	for (size_t i = 0; str[i] != '\0'; ++i) {
		if (prev == ' ' && str[i] != ' ') {
			if (token_len == token_cap) {
				token_cap *= 2;
				token = realloc(token, token_cap * sizeof token[0]);
				if (!token) exit(1);
			}
			token[token_len++] = &str[i];
		}
		else if (prev != ' ' && str[i] == ' ') {
			str[i] = '\0';
		}
		prev = str[i];
	}
	return token_len;
}

enum Command strToCommand(char *str) {
	static const struct {
		enum Command command;
		char *str;
	} COMMAND_STR_TABLE[] = {
		{CMD_SIZE,  "size"},
		{CMD_SAVE,  "save"},
		{CMD_QUIT,  "quit"},
		{CMD_RUN,   "run"},
		{CMD_HALT,  "halt"},
		{CMD_RESET, "reset"},
		{CMD_MEM,   "mem"},
	};
	static const size_t COMMAND_STR_TABLE_LEN = sizeof COMMAND_STR_TABLE / sizeof COMMAND_STR_TABLE[0];
	for (size_t i = 0; i < COMMAND_STR_TABLE_LEN; ++i) {
		if (strcmp(str, COMMAND_STR_TABLE[i].str) == 0) return COMMAND_STR_TABLE[i].command;
	}
	return CMD_NONE;
}

char *getFeedback(void) {
	return "FEEDBACK";
}

void setRunning(bool val) {
	isRunning = val;
}