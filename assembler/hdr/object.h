#ifndef HDR_OBJECT
#define HDR_OBJECT
#include "symbol.h"

struct Segment {
	uint32_t address;
	uint8_t *data;
	size_t data_len, data_cap;
};

struct Object {
	char *path;
	struct Segment *segment;
	size_t segment_len, segment_cap;
};

struct Object *newObject(
	char *path
);

void addExportSymbol(
	struct Object *object,
	enum SymbolType type,
	struct Symbol *scope,
	char *name,
	uint32_t value
);

struct Segment *newSegment(
	struct Object *object,
	uint32_t address
);

void addData(
	struct Segment *segment,
	uint32_t data,
	size_t size
);

void setBranchBackwards(
	struct Segment *segment
);

void addReference(
	struct Segment *segment,
	uint32_t address,
	char *name,
	uint32_t offset,
	size_t size
);

#endif