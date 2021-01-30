#include "memory.h"
#include <stdlib.h>
#include <stdio.h>

static uint8_t *vram, *ram, *bank, *flash, *microcode;

static bool loadMemory(uint8_t *buffer, char *path);

bool initMemory(char *flash_path, char *microcode_path) {
	vram = malloc(32768);
	if (!vram) {
		puts("Could not allocate VRAM");
		return false;
	}

	ram = malloc(16384);
	if (!ram) {
		puts("Could not allocate RAM");
		return false;
	}

	flash = malloc(262144);
	if (!flash) {
		puts("Could not allocate flash memory");
		return false;
	}
	if (!loadMemory(flash, flash_path)) return false;
	bank = flash;
	
	microcode = malloc(3 * 524288);
	if (!microcode) {
		puts("Could not allocate microcode buffer");
		return false;
	}
	if (!loadMemory(microcode, microcode_path)) return false;

	return true;
}

void deinitMemory(void) {
	free(vram);
	free(ram);
	free(flash);
	free(microcode);
}

bool loadMemory(uint8_t *buffer, char *path) {
	return true;
}

uint8_t *getVRAM(void) {
	return vram;
}