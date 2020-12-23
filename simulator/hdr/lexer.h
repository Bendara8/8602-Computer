#ifndef HDR_LEXER
#define HDR_LEXER
#include <stddef.h>

struct TokenVec;

void lexFile(
	char *path,
	struct TokenVec *tok_vec
);

unsigned parseNum(
	char *lexeme
);

unsigned parseHex(
	char *lexeme
);

#endif