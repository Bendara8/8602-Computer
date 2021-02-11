#ifndef HDR_COMMAND
#define HDR_COMMAND

void commandSize(
	char **token,
	size_t token_len
);

void commandSave(
	char **token,
	size_t token_len
);

void commandRun(
	char **token,
	size_t token_len
);

void commandHalt(
	char **token,
	size_t token_len
);

void commandReset(
	char **token,
	size_t token_len
);

void commandMem(
	char **token,
	size_t token_len
);


#endif