#ifndef HDR_ARGUMENTS
#define HDR_ARGUMENTS

bool readArguments(
	char **arg,
	size_t arg_len
);

char *getTargetPath(
	void
);

char *getOutputPath(
	void
);

#endif