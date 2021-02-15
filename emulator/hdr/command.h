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
	void
);

void commandHalt(
	void
);

void commandReset(
	void
);

void commandMem(
	char **token,
	size_t token_len
);

char *getFeedback(
	void
);

void setFeedback(
	char *feedback
);

#endif