#include "symbol.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct Symbol *symbol = NULL;
static size_t symbol_len = 0, symbol_cap = 32;

void initSymbolList(void) {
	symbol = malloc(symbol_cap * sizeof symbol[0]);
	if (symbol == NULL) {
		puts("Unable to allocate symbol list.");
		exit(EXIT_FAILURE);
	}
}

void deinitSymbolList(void) {
	resetSymbolList();
	free(symbol);
}

void resetSymbolList(void) {
	symbol_len = 0;
}

struct Symbol *getSymbolList(void) {
	return symbol;
}

size_t getSymbolLen(void) {
	return symbol_len;
}

struct Symbol *addSymbol(enum SymbolType type, struct Symbol *scope, char *name, uint32_t value) {
	if (symbol_len == symbol_cap) {
		symbol_cap *= 2;
		symbol = realloc(symbol, symbol_cap * sizeof symbol[0]);
		if (symbol == NULL) {
			puts("Unable to reallocate symbol list.");
			exit(EXIT_FAILURE);
		}
	}
	struct Symbol *ret = &symbol[symbol_len++];
	ret->type = type;
	ret->value = value;
	ret->name = name;
	ret->scope = scope;
	return ret;
}

struct Symbol *lookupSymbol(char *name, struct Symbol *scope) {
	for (size_t i = 0; i < symbol_len; ++i) {
		if (
			(symbol[i].scope == NULL || symbol[i].scope == scope) &&
			strcmp(symbol[i].name, name) == 0
		) {
			return &symbol[i];
		}
	}
	return NULL;
}

uint32_t normalizeWord(uint32_t address) {
	return (address & 0x00003FFF) | 0x0000C000;
}

uint32_t normalizeLong(uint32_t address) {
	return (address & 0x00003FFF) | 0x0000C000 | (address & 0x0003C000) << 2;
}