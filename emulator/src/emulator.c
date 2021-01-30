#include "display.h"
#include "memory.h"
#include <stdlib.h>

int32_t main(void) {
	if (!initMemory("res/flash.bin", "res/microcode.bin")) {
		return 1;
	}
	atexit(deinitMemory);
	if (!initDisplay(512, 256, 1, 16666667, getVRAM())) {
		return 1;
	}
	atexit(deinitDisplay);
	for (uint64_t i = 0; i < 1000000000; ++i) {}
	return 0;
}