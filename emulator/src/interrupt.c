#include "interrupt.h"

static bool
	interrupt_in[INTER_LEN] = {false},
	interrupt_out[INTER_LEN] = {false},
	interrupt_reset[INTER_LEN] = {false};

bool getInterrupt(enum Interrupt i) {
	return interrupt_out[(size_t)i];
}

void generateInterrupt(enum Interrupt i) {
	if (interrupt_reset[(size_t)i]) interrupt_in[(size_t)i] = true;
}

void setInterruptReset(enum Interrupt i, bool val) {
	interrupt_reset[(size_t)i] = val;
	if (!val) {
		interrupt_in[(size_t)i] = false;
	}
}

void readInterrupts(void) {
	for (size_t i = 0; i < INTER_LEN; ++i) {
		interrupt_out[i] = interrupt_in[i];
	}
}