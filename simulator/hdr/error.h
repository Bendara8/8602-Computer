#ifndef HDR_ERROR
#define HDR_ERROR
#include <stddef.h>

enum AbortType {
	ABORT_ALLOCATION,
	ABORT_ATEXIT,
};

void raiseAbort(
	enum AbortType type,
	...
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