#ifndef HDR_OBJECT
#define HDR_OBJECT
#include "symbol.h"

struct Segment {
	uint32_t line;
	uint32_t address;
	uint8_t *data;
	size_t data_len, data_cap;
};

struct Object {
	char *path;
	struct Segment *segment;
	size_t segment_len, segment_cap;
	struct Symbol *symbol;
	size_t symbol_len, symbol_cap;
	struct Reference *reference;
	size_t reference_len, reference_cap;
};

void initObjectList(
	void
);

void deinitObjectList(
	void
);

struct Object *newObject(
	char *path
);

struct Object *getObjectList(
	void
);

size_t getObjectLen(
	void
);

void addExportSymbol(
	struct Object *object,
	enum SymbolType type,
	char *name,
	uint32_t value
);

void addReference(
	struct Object *object,
	enum ReferenceType type,
	uint32_t line,
	uint32_t address,
	char *name,
	uint32_t offset,
	size_t size
);

struct Segment *newSegment(
	struct Object *object,
	uint32_t line,
	uint32_t address
);

void addData(
	struct Segment *segment,
	uint32_t data,
	size_t size
);

void resolveReferences(
	struct Object *object,
	struct Symbol *symbol,
	size_t symbol_len,
	uint32_t *err_ct
);

#endif