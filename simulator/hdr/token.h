#ifndef HDR_TOKEN
#define HDR_TOKEN
#include <stddef.h>

enum TokenType {
	TOK_BUS,
	TOK_NET,
	TOK_CHIP,
	TOK_BEG_BRACE,
	TOK_END_BRACE,
	TOK_EQUALS,
	TOK_COLON,
	TOK_COMMA,
	TOK_DASH,
	TOK_SYMBOL,
	TOK_NUM,
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
	size_t cap, len;
};

void initTokenVec(
	struct TokenVec *vec,
	size_t cap
);

struct Token *newToken(
	struct TokenVec *vec
);

#endif