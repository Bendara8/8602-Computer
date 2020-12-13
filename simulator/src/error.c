#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "error.h"

static const char *ABORT_MSG[] = {
	"Could not allocate memory",
	"Could not register function for call at exit",
	"Could not find file '%s'",
};

static const char *ERROR_MSG[] = {
	"Oversize lexeme beginning with '%s' (max length is %zu characters)",
	"'%s' is not a number (numbers must contain only decimal digits)",
};

static char *error_path = "";
static unsigned error_line = 0;

static struct {
	void **vec;
	size_t cap, len;
} free_target = {NULL, 0, 0};

void raiseAbort(enum AbortType type, ...) {
	va_list args;
	va_start(args, type);
	printf("Abort: ");
	vprintf(ABORT_MSG[type], args);
	printf("\n");
	exit(1);
}

void raiseError(enum ErrorType type, ...) {
	va_list args;
	va_start(args, type);
	printf("Error: (%s:%u) ", error_path, error_line);
	vprintf(ERROR_MSG[type], args);
	printf("\n");
}

void setErrorPath(char *path) {
	error_path = path;
}

void setErrorLine(unsigned line) {
	error_line = line;
}

void initFreeTargetVec(size_t cap) {
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