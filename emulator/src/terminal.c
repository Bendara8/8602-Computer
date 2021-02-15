#include "terminal.h"
#include "interface.h"
#include "command.h"
#include "input.h"
#include "draw.h"
#include "cpu.h"
#include "memory.h"
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

enum Window {
	WIN_REGISTERS,
	WIN_CODE,
	WIN_STACK,
	WIN_MEMORY,
	WIN_FLASH,
	WIN_CONSOLE,
};
#define WIN_COUNT 6

static int size[WIN_COUNT] = {0};
static bool visible[WIN_COUNT] = {false};
static bool hidden[WIN_COUNT] = {false};
static enum Window focus = WIN_CONSOLE;

static void haltIfRunning(void);
static void sizeTerminal(int remaining_size);
static int  sizeWindow(enum Window win, int remaining_size, int below_headers);
static int  drawWindow(enum Window win, char *name, char *key, void (*drawFunction)(int), int line);

bool initTerminal(void) {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	size[WIN_REGISTERS] = 13;
	size[WIN_CODE] = 11;
	size[WIN_STACK] = 11;
	size[WIN_MEMORY] = 11;
	size[WIN_FLASH] = 11;
	size[WIN_CONSOLE] = 3;
	visible[WIN_CONSOLE] = true;
	if (!initInput()) return false;
	atexit(&deinitInput);
	return true;
}

void sizeTerminal(int remaining_size) {
	remaining_size = sizeWindow(WIN_CODE,      remaining_size, 4);
	remaining_size = sizeWindow(WIN_REGISTERS, remaining_size, 3);
	remaining_size = sizeWindow(WIN_STACK,     remaining_size, 2);
	remaining_size = sizeWindow(WIN_MEMORY,    remaining_size, 1);
	remaining_size = sizeWindow(WIN_FLASH,     remaining_size, 0);
}

int sizeWindow(enum Window win, int remaining_size, int below_headers) {
	if (!hidden[win] && remaining_size >= size[win] + below_headers) {
		visible[win] = true;
		return remaining_size - size[win];
	}
	else {
		visible[win] = false;
		return remaining_size - 1;
	}
}

void deinitTerminal(void) {
	endwin();
}

void updateTerminal(void) {
	int ch = getch();
	if (ch != ERR) {
		switch (ch) {
			case KEY_F(7): focus = WIN_REGISTERS; haltIfRunning(); drawTerminal(); break;
			case KEY_F(6): focus = WIN_CODE;      haltIfRunning(); drawTerminal(); break;
			case KEY_F(5): focus = WIN_STACK;     haltIfRunning(); drawTerminal(); break;
			case KEY_F(4): focus = WIN_MEMORY;    haltIfRunning(); drawTerminal(); break;
			case KEY_F(3): focus = WIN_FLASH;     haltIfRunning(); drawTerminal(); break;
			case KEY_F(2): focus = WIN_CONSOLE;   drawTerminal(); break;
			default: switch (focus) {
				case WIN_REGISTERS: inputRegisters(ch); drawTerminal(); break;
				case WIN_CODE:      inputCode(ch);      drawTerminal(); break;
				case WIN_STACK:     inputStack(ch);     drawTerminal(); break;
				case WIN_MEMORY:    inputMemory(ch);    drawTerminal(); break;
				case WIN_FLASH:     inputFlash(ch);     drawTerminal(); break;
				case WIN_CONSOLE:   inputConsole(ch);   drawConsoleLine(); break;
			}
		}
	}
}

void haltIfRunning(void) {
	if (getRunning()) {
		setRunning(false);
		setFeedback("Halted system.");
	}
}

void drawTerminal(void) {
	sizeTerminal(LINES - size[WIN_CONSOLE]);
	erase();
	int32_t line = 0;
	line = drawWindow(WIN_REGISTERS, "registers", "F7", &drawRegisters, line);
	line = drawWindow(WIN_CODE, "code", "F6", &drawCode, line);
	line = drawWindow(WIN_STACK, "stack", "F5", &drawStack, line);
	line = drawWindow(WIN_MEMORY, "memory", "F4", &drawMemory, line);
	line = drawWindow(WIN_FLASH, "flash", "F3", &drawFlash, line);
	line = drawWindow(WIN_CONSOLE, "console", "F2", &drawConsole, line);
	move(LINES - 1, getConsolePos() + 2);
	curs_set(focus == WIN_CONSOLE ? 1 : 0);
	refresh();
}

int drawWindow(enum Window win, char *name, char *key, void (*drawFunction)(int), int line) {
	move(line, 0);
	size_t len = COLS - strlen(name) - strlen(key) - 3;
	for (size_t x = 0; x < len; ++x) addch('-');
	addch(focus == win ? '(' : ' ');
	addstr(name);
	addch(focus == win ? ')' : ' ');
	addstr(key);
	if (visible[win]) {
		(*drawFunction)(line);
		return line + size[win];
	}
	return line + 1;
}