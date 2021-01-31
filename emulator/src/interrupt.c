#include "interrupt.h"

static bool interrupt_in[INTER_LEN], interrupt_out[INTER_LEN];

bool getInterrupt(enum Interrupt i) {
	return interrupt_out[(size_t)i];
}

void generateInterrupt(enum Interrupt i) {
	interrupt_in[(size_t)i] = true;
}

void resetInterrupt(enum Interrupt i) {
	interrupt_in[(size_t)i] = false;
}

void readInterrupts(void) {
	for (size_t i = 0; i < INTER_LEN; ++i) {
		interrupt_out[i] = interrupt_in[i];
	}
}