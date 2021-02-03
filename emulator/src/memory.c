#include "memory.h"
#include "keyboard.h"
#include "interrupt.h"
#include <stdlib.h>
#include <stdio.h>

static const size_t
	VRAM_SIZE      = 32768,
	RAM_SIZE       = 16128,
	IO_SIZE        = 256,
	FLASH_SIZE     = 262144,
	BANK_SIZE      = 16384,
	MICROCODE_SIZE = 3 * 524288,
	VRAM_START     = 0,
	RAM_START      = VRAM_START + VRAM_SIZE,
	IO_START       = RAM_START + RAM_SIZE,
	BANK_START     = IO_START + IO_SIZE;

static uint8_t *vram, *ram, *bank, *flash, *microcode;
static uint8_t status_led;

static bool loadMemory(uint8_t *buffer, size_t size, char *path);
uint8_t readIO(enum IO select);
void writeIO(uint8_t data, enum IO select);

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
	if (address < VRAM_START + VRAM_SIZE) return vram[address & 0x7FFF];
	if (address < RAM_START + RAM_SIZE)   return ram[address & 0x3FFF];
	if (address < IO_START + IO_SIZE)     return readIO((uint8_t)(address & 0x0007));
	if (address < BANK_START + BANK_SIZE) return bank[address & 0x3FFF];
	return 0;
}

void writeMemory(uint8_t data, uint16_t address) {
	if (address < VRAM_START + VRAM_SIZE) vram[address & 0x7FFF] = data;
	if (address < RAM_START + RAM_SIZE)   ram[address & 0x3FFF] = data;
	if (address < IO_START + IO_SIZE)     writeIO(data, (uint8_t)(address & 0x0007));
	if (address < BANK_START + BANK_SIZE) bank[address & 0x3FFF] = data;
}

uint8_t readIO(enum IO select) {
	switch (select) {
		case IO_INPUT:
			return (
				((uint8_t)getInterrupt(INTER_0) << 0) |
				((uint8_t)getInterrupt(INTER_1) << 1) |
				(status_led) |
				((uint8_t)getKeyClock() << 6) |
				((uint8_t)getKeyData() << 7)
			);
			break;
		case IO_OUTPUT: return 0;
		default: return 0;
	}
}

void writeIO(uint8_t data, enum IO select) {
	switch (select) {
		case IO_INPUT: break;
		case IO_OUTPUT:
			setInterruptReset(INTER_0, data & OUT_CLKINTRST);
			setInterruptReset(INTER_1, data & OUT_KEYINTRST);
			status_led = data & OUT_STATUSLED;
			pullKeyClockLow(!(data & OUT_KEYCLK));
			pullKeyDataLow(!(data & OUT_KEYDAT));
			break;
	}
}

uint8_t readVRAM(uint16_t address) {
	if (address < VRAM_SIZE) return vram[address];
	return 0;
}

uint8_t readMicrocode(uint32_t address) {
	if (address < MICROCODE_SIZE) return microcode[address];
	return 0;
}