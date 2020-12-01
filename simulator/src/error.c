#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "error.h"

void raiseAbort(enum AbortType type, ...) {
	va_list args;
	va_start(args, type);
	printf("Abort: ");
	vprintf(ABORT_MSG[type], args);
	printf("\n");
	exit(1);
}

static struct {
	void **vec = NULL;
	size_t cap = 0, len = 0;
} free_target;

void initTargetVec(size_t cap) {
	free_target.cap = cap;
	free_target.len = 0;
	free_target.vec = malloc(sizeof (void *) * free_target.cap);
	if (!free_target.vec) raiseAbort(
		ABORT_ALLOCATION
	);
	if (atexit(freeTargetVec)) raiseAbort(
		ABORT_ATEXIT
	);
}

void freeTargetVec(void) {
	for (size_t i = 0; i < free_target.len; ++i) {
		free(free_target.vec[i]);
	}
	free(free_target.vec);
}

void addFreeTarget(void *target) {
	if (free_target.len == free_target.cap) {
		free_target.cap *= 2;
		free_target.vec = realloc(free_target.vec, sizeof (void *) * free_target.cap);
		if (!free_target.vec) raiseAbort(
			ABORT_ALLOCATION
		);
	}
	free_target.vec[free_target.len] = target;
	++free_target.len;
}

const char *ABORT_MSG[] = {
	"Could not allocate memory",
	"Could not register function for call at exit",
};