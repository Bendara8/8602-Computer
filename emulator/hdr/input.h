#ifndef HDR_INPUT
#define HDR_INPUT

bool initInput(
	void
);

void deinitInput(
	void
);

void inputRegisters(
	int ch
);

void inputCode(
	int ch
);

void inputStack(
	int ch
);

void inputMemory(
	int ch
);

void inputFlash(
	int ch
);

void inputConsole(
	int ch
);

int getConsolePos(
	void
);

char *getInput(
	void
);

#endif