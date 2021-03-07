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

enum ReferenceType {
	REF_RESOLVED,
	REF_DATA,
	REF_RELATIVE,
	REF_ABSOLUTE,
};

struct Symbol {
	enum SymbolType type;
	char *name;
	uint32_t value;
	struct Symbol *scope;
};

struct Reference {
	enum ReferenceType type;
	uint32_t line;
	char *name;
	uint32_t address;
	uint32_t offset;
	size_t size;
};

void initSymbolList(
	void
);

void deinitSymbolList(
	void
);

void resetSymbolList(
	void
);

struct Symbol *getSymbolList(
	void
);

size_t getSymbolLen(
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

uint32_t normalizeWord(
	uint32_t address
);

uint32_t normalizeLong(
	uint32_t address
);

#endif