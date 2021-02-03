#ifndef HDR_MEMORY
#define HDR_MEMORY

enum IO {
	IO_INPUT,
	IO_OUTPUT,
};

enum OutReg {
	OUT_CLKINTRST = 0x01,
	OUT_KEYINTRST = 0x02,
	OUT_STATUSLED = 0x04,
	OUT_KEYCLK    = 0x40,
	OUT_KEYDAT    = 0x80,
};

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