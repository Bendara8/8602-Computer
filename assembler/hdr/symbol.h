#ifndef HDR_SYMBOL
#define HDR_SYMBOL

enum SymbolType {
	SYM_SCOPE,
	SYM_LABEL,
	SYM_NUMBER,
	SYM_B_PLUS_IMM,
	SYM_B_MINUS_IMM,
	SYM_P_PLUS_IMM,
};

struct Symbol {
	enum SymbolType type;
	uint32_t value;
	char *name;
	struct Symbol *scope;
};

void resetSymbolList(
	void
);

struct Symbol *addSymbol(
	enum SymbolType type,
	struct Symbol *scope,
	char *name,
	uint32_t value
);

struct Symbol *lookupSymbol(
	char *name,
	struct Symbol *scope
);

#endif