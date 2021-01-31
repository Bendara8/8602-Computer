#ifndef HDR_INTERRUPT
#define HDR_INTERRUPT

enum Interrupt {
	INTER_0,
	INTER_1,
	INTER_LEN,
};

void
generateInterrupt(
	enum Interrupt interrupt
);

#endif