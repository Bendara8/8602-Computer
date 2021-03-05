#ifndef HDR_INSTRUCTION
#define HDR_INSTRUCTION
#include "pattern.h"

uint32_t getOpcode(
	struct Pattern *pattern
);

void addArgData(
	struct Pattern *pattern
);

#endif