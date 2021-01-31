#include "memory.h"
#include <stdlib.h>
#include <stdio.h>

static const size_t VRAM_SIZE = 32768, RAM_SIZE = 16384;
static const size_t FLASH_SIZE = 262144, MICROCODE_SIZE = 3 * 524288;

static uint8_t *vram, *ram, *bank, *flash, *microcode;

static bool loadMemory(uint8_t *buffer, size_t size, char *path);

bool initMemory(char *flash_path, char *microcode_path) {
	vram = malloc(VRAM_SIZE);
	if (!vram) {
		puts("Could not allocate VRAM");
		return false;
	}

	ram = malloc(RAM_SIZE);
	if (!ram) {
		puts("Could not allocate RAM");
		return false;
	}

	flash = malloc(FLASH_SIZE);
	if (!flash) {
		puts("Could not allocate flash memory");
		return false;
	}
	if (!loadMemory(flash, FLASH_SIZE, flash_path)) return false;
	bank = flash;
	
	microcode = malloc(MICROCODE_SIZE);
	if (!microcode) {
		puts("Could not allocate microcode buffer");
		return false;
	}
	if (!loadMemory(microcode, MICROCODE_SIZE, microcode_path)) return false;

	return true;
}

bool loadMemory(uint8_t *buffer, size_t size, char *path) {
	FILE *file = fopen(path, "rb");
	if (!file) {
		printf("Unable to open '%s'\n", path);
		return false;
	}
	for (size_t i = 0; i < size; ++i) {
		int byte = getc(file);
		if (byte == EOF) {
			printf("File '%s' is too small, expected %zu bytes\n", path, size);
			fclose(file);
			return false;
		}
		buffer[i] = (uint8_t)byte;
	}
	if (getc(file) != EOF) {
		printf("File '%s' is too large, expected %zu bytes\n", path, size);
		fclose(file);
		return false;
	}
	fclose(file);
	return true;
}

void deinitMemory(void) {
	free(vram);
	free(ram);
	free(flash);
	free(microcode);
}

uint8_t readMemory(uint16_t address) {
	return 0;
}

void writeMemory(uint8_t data, uint16_t address) {

}

uint8_t readVRAM(uint16_t address) {
	if (address < VRAM_SIZE) return vram[address];
	return 0;
}

uint8_t readMicrocode(uint32_t address) {
	if (address < MICROCODE_SIZE) return microcode[address];
	return 0;
}