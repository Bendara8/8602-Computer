#include "object.h"
#include "symbol.h"

struct Object *newObject(char *path) {
	return NULL;
}

void addExportSymbol(
	struct Object *object,
	enum SymbolType type,
	struct Symbol *scope,
	char *name,
	uint32_t value
) {

}

struct Segment *newSegment(struct Object *object, uint32_t address) {
	return NULL;
}

void addData(struct Segment *segment, uint32_t data, size_t size) {

}

void setBranchBackwards(struct Segment *segment) {

}

void addReference(struct Segment *segment, uint32_t address, char *name, uint32_t offset, size_t size) {

}