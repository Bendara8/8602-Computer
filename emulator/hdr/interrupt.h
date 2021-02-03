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

void setInterruptReset(
	enum Interrupt interrupt,
	bool value
);

void readInterrupts(
	void
);

#endif