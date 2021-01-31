#include "interrupt.h"

static bool interrupt[INTER_LEN];

void generateInterrupt(enum Interrupt i) {
	interrupt[(size_t)i] = true;
}

bool getInterrupt(enum Interrupt i) {
	return interrupt[(size_t)i];
}

void resetInterrupt(enum Interrupt i) {
	interrupt[(size_t)i] = false;
}