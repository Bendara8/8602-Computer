#ifndef HDR_CPU
#define HDR_CPU
#include "alu.h"
#include "control.h"

struct CPUState {
	uint8_t
		step,
		reset,
		accumulator,
		index,
		bank,
		opcode;
	uint16_t
		pointer,
		base_pointer,
		stack_pointer,
		instruction_pointer,
		transfer_pointer;
	enum Flag flags;
	enum Control control;
};

void resetCPU(
	void
);

void clockCPU(
	void
);

const struct CPUState *getCPU(
	void
);

#endif