#ifndef HDR_INTERRUPT
#define HDR_INTERRUPT

enum Interrupt {
	INTER_0,
	INTER_1,
	INTER_LEN,
};

bool getInterrupt(
	enum Interrupt interrupt
);

void generateInterrupt(
	enum Interrupt interrupt
);

void resetInterrupt(
	enum Interrupt interrupt
);

void readInterrupts(
	void
);

#endif