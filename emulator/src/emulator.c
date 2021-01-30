#include "display.h"
#include "memory.h"

int32_t main(void) {
	if (!initMemory("res/flash.bin", "res/microcode.bin")) {
		return 1;
	}
	if (!initDisplay(512, 256, 1, 16666667, getVRAM())) {
		return 1;
	}
	for (uint64_t i = 0; i < 10000000000; ++i) {}
	return 0;
}