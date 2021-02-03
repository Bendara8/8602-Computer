#include "arguments.h"
#include "display.h"
#include "keyboard.h"
#include "cpu.h"
#include "memory.h"
#include <stdlib.h>

static const uint64_t CLOCK_PERIOD = 400; // clock period (ns)

int32_t main(int arg_len, char **arg) {
	if (!readArguments(arg, (size_t)arg_len)) return 1;

	if (!initMemory(getFlashPath(), getMicrocodePath())) return 1;
	atexit(deinitMemory);

	if (!initDisplay(512, 256, 1, 16666667)) return 1;
	atexit(deinitDisplay);

	if (!initKeyboard()) return 1;
	atexit(deinitKeyboard);
	
	resetCPU();
	while (1) {
		stepKeyboard(CLOCK_PERIOD);
		stepCPU();
		stepDisplay(CLOCK_PERIOD);
	}
	return 0;
}