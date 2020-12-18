#ifndef HDR_TOKEN
#define HDR_TOKEN
#include <stddef.h>

enum TokenType {
	TOK_NONE,
	TOK_NULL,
	TOK_BUS,
	TOK_NET,
	TOK_CHIP,
	TOK_IN,
	TOK_OUT,
	TOK_SYMBOL,
	TOK_NUM,
	TOK_BEG_BRACE,
	TOK_END_BRACE,
	TOK_EQUALS,
	TOK_COLON,
	TOK_COMMA,
	TOK_DASH,
};

struct Token {
	enum TokenType type;
	unsigned line;
	union {
		char *str;
		unsigned num;
	};
};

struct TokenVec {
	struct Token *buf;
	size_t len, cap;
};

struct TokenQue {
	struct TokenVec *vec;
	size_t pos;
};

void initTokenVec(
	struct TokenVec *vec,
	size_t cap
);

void initTokenQue(
	struct TokenQue *que,
	struct TokenVec *vec
);

struct Token *newToken(
	struct TokenVec *vec
);

struct Token *currToken(
	struct TokenQue *que
);

struct Token *nextToken(
	struct TokenQue *que
);

struct Token *peekToken(
	struct TokenQue *que
);

enum TokenType peekTokenType(
	struct TokenQue *tok_que
);

void assertNextTokenType(
	struct TokenQue *tok_que,
	enum TokenType type
);

#endif