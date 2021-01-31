#ifndef HDR_MEMORY
#define HDR_MEMORY

bool initMemory(
	char *flash_path,
	char *microcode_path
);

void deinitMemory(
	void
);

uint8_t readMemory(
	uint16_t address
);

void writeMemory(
	uint8_t data,
	uint16_t address
);

uint8_t readVRAM(
	uint16_t address
);

uint8_t readMicrocode(
	uint32_t address
);

#endif