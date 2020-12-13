#ifndef HDR_ERROR
#define HDR_ERROR
#include <stddef.h>

enum AbortType {
	ABORT_ALLOCATION,
	ABORT_ATEXIT,
	ABORT_NO_FILE,
};

enum ErrorType {
	ERROR_LONG_LEXEME,
	ERROR_NOT_NUM,
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

void initFreeTargetVec(
	size_t cap
);

void freeTargetVec(
	void
);

void addFreeTarget(
	void *target
);

#endif