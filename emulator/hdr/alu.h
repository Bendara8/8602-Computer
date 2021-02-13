#ifndef HDR_ALU
#define HDR_ALU
#include "control.h"

enum Flag {
	FLAG_Z = 0x1,
	FLAG_N = 0x2,
	FLAG_C = 0x4, 
	FLAG_I = 0x8,
};

uint8_t calcALU(
	uint8_t in_a,
	uint8_t in_b,
	enum Flag *flags,
	enum Control control
);

#endif