#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "error.h"

static const char *ABORT_MSG[] = {
	"Could not allocate memory",
	"Could not register function for call at exit",
	"Could not find file '%s'",
	"Invalid arguments (expected single circuit definition file)",
	"Missing free target",
	"Fatal error",
};

static const char *ERROR_MSG[] = {
	"Oversize lexeme beginning with '%s' (max length is %zu characters)",
	"'%s' is not a number (numbers must contain only decimal digits)",
	"'%s' is not a hexadecimal humber\n       (hexadecimal numbers must contain only hexadecimal digits and start with '$')",
	"Expected a number",
	"Expected a hexadecimal number",
	"'%s' is not a chip",
	"Unexpected token",
	"Unexpected end of file",
	"'%i' is an invalid value in this context",
	"Too many nets (max %zu)",
	"Unmatched quantity of nets",
	"'%s' is not a net",
	"'%s' is not a bus",
	"Command contains too many parts (max is %zu parts)",
	"'%s' is not a command",
	"'%s' is an invalid option",
	"Incorrect number of arguments for %s command",
	"'%s' does not have any onboard memory",
};

static char *error_path = "";
static unsigned error_line = 0;
static int error_flag = 0;

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
	if (error_line == 0) printf("Error: ");
	else printf("Error: (%s:%u) ", error_path, error_line);
	vprintf(ERROR_MSG[type], args);
	printf("\n");
	error_flag = 1;
}

void setErrorPath(char *path) {
	error_path = path;
}

void setErrorLine(unsigned line) {
	error_line = line;
}

void setErrorFlag(int flag) {
	error_flag = flag;
}

int getErrorFlag(void) {
	return error_flag;
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

void **findFreeTarget(void *target) {
	for (size_t i = 0; i < free_target.len; ++i) {
		if (target == free_target.vec[i]) return &free_target.vec[i];
	}
	return NULL;
}