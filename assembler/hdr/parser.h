#ifndef HDR_PARSER
#define HDR_PARSER
#include "token.h"

void parsePatternsIntoObject(
	char *path
);

uint32_t parseNumber(
	struct Token *token
);

void error(
	char *format,
	...
);

char *dupStr(
	char *str
);

#endif