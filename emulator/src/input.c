#include "input.h"
#include "interface.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <ncurses.h>

static const size_t INIT_INPUT_CAP = 128;

static char *input;
static size_t input_pos, input_cap;

void deleteChar(void);
void insertChar(char ch);
bool resizeInput(size_t new_len);

bool initInput(void) {
	input_cap = INIT_INPUT_CAP;
	input = malloc(input_cap * sizeof input[0]);
	if (!input) return false;
	input[0] = '\0';
	input_pos = 0;
	return true;
}

void deinitInput(void) {
	free(input);
}

void inputRegisters(int ch) {

}

void inputCode(int ch) {
	switch (ch) {
		case KEY_UP:    clockSystem();      break;
		case KEY_RIGHT: stepInstruction();  break;
		case KEY_DOWN:  nextInstruction();  break;
		case KEY_LEFT:  finishSubroutine(); break;
		default: break;
	}
}

void inputStack(int ch) {

}

void inputMemory(int ch) {

}

void inputFlash(int ch) {

}

void inputConsole(int ch) {
	switch (ch) {
		case KEY_LEFT:
			if (input_pos > 0) --input_pos;
			break;

		case KEY_RIGHT:
			if (input_pos < strlen(input)) ++input_pos;
			break;

		case KEY_HOME:
			input_pos = 0;
			break;

		case KEY_END:
			input_pos = strlen(input);
			break;

		case KEY_DC:
			deleteChar();
			break;

		case KEY_BACKSPACE:
			if (input_pos > 0) {
				--input_pos;
				deleteChar();
			}
			break;

		case '\n':
			runCommand(input);
			input[0] = '\0';
			input_pos = 0;
			break;

		default:
			if (isprint(ch)) insertChar((char)ch);
			break;
	}
}

void deleteChar(void) {
	for (size_t i = input_pos; input[i] != '\0'; ++i) {
		input[i] = input[i + 1];
	}
}

void insertChar(char ch) {
	size_t len = strlen(input);
	resizeInput(len + 2);
	for (size_t i = len; i >= input_pos && i <= len; --i) {
		input[i + 1] = input[i];
	}
	input[input_pos++] = ch;
}

bool resizeInput(size_t new_len) {
	if (new_len >= input_cap) {
		input_cap = new_len * 2;
		input = realloc(input, input_cap);
		if (!input) exit(1);
		return true;
	}
	return false;
}

int getConsolePos(void) {
	return (int)input_pos;
}

char *getInput(void) {
	return input;
}