#include "interrupt.h"

static bool interrupt[INTER_LEN];

bool getInterrupt(enum Interrupt i) {
	return interrupt[(size_t)i];
}

void generateInterrupt(enum Interrupt i) {
	interrupt[(size_t)i] = true;
}

void resetInterrupt(enum Interrupt i) {
	interrupt[(size_t)i] = false;
}