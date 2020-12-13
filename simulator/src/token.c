#include <stdlib.h>
#include "token.h"
#include "error.h"

void initTokenVec(struct TokenVec *vec, size_t cap) {
	vec->buf = malloc(sizeof (struct Token) * cap);
	if (!vec->buf) raiseAbort(
		ABORT_ALLOCATION
	);
	vec->cap = cap;
	vec->len = 0;
}

struct Token *newToken(struct TokenVec *vec) {
	if (vec->len == vec->cap) {
		vec->cap *= 2;
		vec->buf = realloc(vec->buf, sizeof (struct Token) * vec->cap);
		if (!vec->buf) raiseAbort(
			ABORT_ALLOCATION
		);
	}
	return &vec->buf[vec->len++];
}