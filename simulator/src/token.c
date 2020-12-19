#include <stdlib.h>
#include "token.h"
#include "error.h"

void initTokenVec(struct TokenVec *vec, size_t cap) {
	vec->buf = malloc(sizeof (struct Token) * cap);
	if (!vec->buf) raiseAbort(
		ABORT_ALLOCATION
	);
	addFreeTarget(vec->buf);
	vec->cap = cap;
	vec->len = 0;
}

void initTokenQue(struct TokenQue *que, struct TokenVec *vec) {
	que->vec = vec;
	que->pos = 0;
}

struct Token *newToken(struct TokenVec *vec) {
	if (vec->len == vec->cap) {
		vec->cap *= 2;
		void **target = findFreeTarget(vec->buf);
		vec->buf = realloc(vec->buf, sizeof (struct Token) * vec->cap);
		if (!vec->buf) raiseAbort(
			ABORT_ALLOCATION
		);
		*target = vec->buf;
	}
	return &vec->buf[vec->len++];
}

struct Token *currToken(struct TokenQue *que) {
	return (que->pos < que->vec->len) ? &que->vec->buf[que->pos] : NULL;
}

struct Token *nextToken(struct TokenQue *que) {
	return (++que->pos < que->vec->len) ? &que->vec->buf[que->pos] : NULL;
}

struct Token *peekToken(struct TokenQue *que) {
	return (que->pos + 1 < que->vec->len) ? &que->vec->buf[que->pos + 1] : NULL;
}

enum TokenType peekTokenType(struct TokenQue *tok_que) {
	return peekToken(tok_que) ? peekToken(tok_que)->type : TOK_NONE;
}

void assertNextTokenType(struct TokenQue *tok_que, enum TokenType type) {
	if (!peekToken(tok_que)) {
		setErrorLine(currToken(tok_que)->line);
		raiseError(ERROR_UNEXPECTED_EOF);
		raiseAbort(ABORT_FATAL_ERROR);
	}
	else if (nextToken(tok_que)->type != type) {
		setErrorLine(currToken(tok_que)->line);
		raiseError(ERROR_UNEXPECTED_TOKEN);
		raiseAbort(ABORT_FATAL_ERROR);
	}
}