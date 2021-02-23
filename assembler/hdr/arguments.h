#ifndef HDR_ARGUMENTS
#define HDR_ARGUMENTS

void initArguments(
	void
);

void deinitArguments(
	void
);

void readArguments(
	char **arg,
	size_t arg_len
);

size_t getInputLen(
	void
);

char *getInput(
	size_t idx
);

char *getOuput(
	void
);

#endif