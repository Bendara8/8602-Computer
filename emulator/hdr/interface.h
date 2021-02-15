#ifndef HDR_INTERFACE
#define HDR_INTERFACE

void enterInterface(
	void
);

void clockSystem(
	void
);

void stepInstruction(
	void
);

void nextInstruction(
	void
);

void finishSubroutine(
	void
);

void runCommand(
	char *str
);

bool getRunning(
	void
);

void setRunning(
	bool val
);

#endif