#include "arguments.h"
#include "interface.h"
#include "display.h"
#include "keyboard.h"
#include "cpu.h"
#include "memory.h"
#include <stdlib.h>

int main(int arg_len, char **arg) {
	if (!readArguments(arg, (size_t)arg_len)) return 1;

	if (!initMemory(getFlashPath(), getMicrocodePath())) return 1;
	atexit(&deinitMemory);

	if (!initDisplay(512, 256, 1, 16666667)) return 1;
	atexit(&deinitDisplay);

	if (!initKeyboard()) return 1;
	atexit(&deinitKeyboard);
	
	resetCPU();
	enterInterface();
	return 0;
}