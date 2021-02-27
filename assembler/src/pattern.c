#include "pattern.h"
#include <stdlib.h>
#include <stdio.h>

static struct Pattern *pattern = NULL;
static size_t pattern_len = 0, pattern_cap = 128;
static size_t pattern_pos = 0;

void initPatternQueue(void) {
	pattern = malloc(pattern_cap * sizeof pattern[0]);
	if (pattern == NULL) {
		puts("Unable to allocate pattern queue.");
		exit(EXIT_FAILURE);
	}
}

void deinitPatternQueue(void) {
	resetPatternQueue();
	free(pattern);
}

void resetPatternQueue(void) {
	pattern_len = 0;
	pattern_pos = 0;
}

struct Pattern *newPattern(void) {
	if (pattern_len == pattern_cap) {
		pattern_cap *= 2;
		pattern = realloc(pattern, pattern_cap * sizeof pattern[0]);
		if (pattern == NULL) {
			puts("Unable to reallocate pattern queue.");
			exit(EXIT_FAILURE);
		}
	}
	return &pattern[pattern_len++];
}