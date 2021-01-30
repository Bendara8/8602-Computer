#ifndef HDR_MEMORY
#define HDR_MEMORY

bool
initMemory(
	char *flash_path,
	char *microcode_path
);

void
deinitMemory(
	void
);

uint8_t *
getVRAM(
	void
);

#endif