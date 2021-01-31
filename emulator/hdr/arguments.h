#ifndef HDR_ARGUMENTS
#define HDR_ARGUMENTS

bool readArguments(
	char **arg,
	size_t arg_len
);

char *getFlashPath(
	void
);

char *getMicrocodePath(
	void
);

bool getRun(
	void
);

#endif