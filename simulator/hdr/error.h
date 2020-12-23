#ifndef HDR_ERROR
#define HDR_ERROR
#include <stddef.h>

enum AbortType {
	ABORT_ALLOCATION,
	ABORT_ATEXIT,
	ABORT_NO_FILE,
	ABORT_NUM_ARGS,
	ABORT_NO_FREE_TARGET,
	ABORT_FATAL_ERROR,
};

enum ErrorType {
	ERROR_LONG_LEXEME,
	ERROR_NOT_NUM,
	ERROR_NOT_HEX,
	ERROR_NO_NUM,
	ERROR_NO_HEX,
	ERROR_NOT_CHIP,
	ERROR_UNEXPECTED_TOKEN,
	ERROR_UNEXPECTED_EOF,
	ERROR_INVALID_NUM,
	ERROR_MANY_NETS,
	ERROR_NUM_NETS,
	ERROR_NOT_NET,
	ERROR_NOT_BUS,
	ERROR_LONG_COMMAND,
	ERROR_NOT_COMMAND,
	ERROR_INVALID_OPTION,
	ERROR_NUM_COMMAND_ARGS,
	ERROR_NO_MEM,
};

void raiseAbort(
	enum AbortType type,
	...
);

void raiseError(
	enum ErrorType type,
	...
);

void setErrorPath(
	char *path
);

void setErrorLine(
	unsigned line
);

void setErrorFlag(
	int flag
);

int getErrorFlag(
	void
);

void initFreeTargetVec(
	size_t cap
);

void freeTargetVec(
	void
);

void addFreeTarget(
	void *target
);

void **findFreeTarget(
	void *target
);

#endif