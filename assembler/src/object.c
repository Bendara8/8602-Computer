#include "object.h"
#include "symbol.h"

static struct Object *object = NULL;
static size_t object_len = 0, object_cap = 16;

void initObjectList(void) {
	object = malloc(object_cap * sizeof object[0]);
	if (object == NULL) {
		puts("Unable to allocate object list.");
		exit(EXIT_FAILURE);
	}
}

void deinitObjectList(void) {
	for (size_t i = 0; i < object_len; ++i) {
		for (size_t j = 0; j < object[i].segment_len; ++j) {
			free(object[i].segment[j].data);
		}
		free(object[i].segment);
	}
	free(object);
}

struct Object *newObject(char *path) {
	return NULL;
}

void addExportSymbol(
	struct Object *object,
	enum SymbolType type,
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