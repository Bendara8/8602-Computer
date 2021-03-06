#include "object.h"
#include "symbol.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void insertData(uint8_t *target, uint32_t data, size_t size);
static void fillReference(
	struct Object *object,
	struct Reference *reference,
	uint32_t symbol,
	uint32_t *err_ct
);

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
		for (size_t j = 0; j < object[i].symbol_len; ++j) {
			free(object[i].symbol[j].name);
		}
		free(object[i].symbol);
		for (size_t j = 0; j < object[i].reference_len; ++j) {
			free(object[i].reference[j].name);
		}
		free(object[i].reference);
	}
	free(object);
}

struct Object *newObject(char *path) {
	if (object_len == object_cap) {
		object_cap *= 2;
		object = realloc(object, object_cap * sizeof object[0]);
		if (object == NULL) {
			puts("Unable to reallocate object list.");
			exit(EXIT_FAILURE);
		}
	}
	struct Object *ret = &object[object_len];
	ret->path = path;
	ret->segment_cap = 16;
	ret->segment = malloc(ret->segment_cap * sizeof ret->segment[0]);
	if (ret->segment == NULL) {
		puts("Unable to allocate object segment list.");
		exit(EXIT_FAILURE);
	}
	ret->segment_len = 0;
	ret->symbol_cap = 16;
	ret->symbol = malloc(ret->symbol_cap * sizeof ret->symbol[0]);
	if (ret->symbol == NULL) {
		puts("Unable to allocate object export symbol list.");
		free(ret->segment);
		exit(EXIT_FAILURE);
	}
	ret->symbol_len = 0;
	ret->reference_cap = 32;
	ret->reference = malloc(ret->reference_cap * sizeof ret->reference[0]);
	if (ret->reference == NULL) {
		puts("Unable to allocate object reference list.");
		free(ret->segment);
		free(ret->symbol);
		exit(EXIT_FAILURE);
	}
	ret->reference_len = 0;
	++object_len;
	return ret;
}

void addExportSymbol(
	struct Object *object,
	enum SymbolType type,
	char *name,
	uint32_t value
) {
	if (object->symbol_len == object->symbol_cap) {
		object->symbol_cap *= 2;
		object->symbol = realloc(object->symbol, object->symbol_cap * sizeof object->symbol[0]);
		if (object->symbol == NULL) {
			puts("Unable to reallocate object export symbol list.");
			exit(EXIT_FAILURE);
		}
	}
	struct Symbol *symbol = &object->symbol[object->symbol_len++];
	symbol->type = type;
	symbol->name = name;
	symbol->value = value;
	symbol->scope = NULL;
}

void addReference(
	struct Object *object,
	enum ReferenceType type,
	uint32_t address,
	char *name,
	uint32_t offset,
	size_t size
) {
	if (object->reference_len == object->reference_cap) {
		object->reference_cap *= 2;
		object->reference = realloc(object->reference, object->reference_cap * sizeof object->reference[0]);
		if (object->reference == NULL) {
			puts("Unable to reallocate object reference list.");
			exit(EXIT_FAILURE);
		}
	}
	struct Reference *reference = &object->reference[object->reference_len++];
	reference->type = type;
	reference->name = name;
	reference->address = address;
	reference->offset = offset;
	reference->size = size;
}

struct Segment *newSegment(struct Object *object, uint32_t address) {
	if (object->segment_len == object->segment_cap) {
		object->segment_cap *= 2;
		object->segment = realloc(object->segment, object->segment_cap * sizeof object->segment[0]);
		if (object->segment == NULL) {
			puts("Unable to reallocate object segment list.");
			exit(EXIT_FAILURE);
		}
	}
	struct Segment *segment = &object->segment[object->segment_len];
	segment->address = address;
	segment->data_cap = 256;
	segment->data = malloc(segment->data_cap * sizeof segment->data[0]);
	if (segment->data == NULL) {
		puts("Unable to allocate segment data.");
		exit(EXIT_FAILURE);
	}
	segment->data_len = 0;
	++object->segment_len;
	return segment;
}

void addData(struct Segment *segment, uint32_t data, size_t size) {
	if (segment->data_len + size > segment->data_cap) {
		segment->data_cap *= 2;
		segment->data = realloc(segment->data, segment->data_cap * sizeof segment->data[0]);
		if (segment->data == NULL) {
			puts("Unable to reallocate segment data.");
			exit(EXIT_FAILURE);
		}
	}
	for (size_t i = 0; i < size; ++i) {
		segment->data[segment->data_len++] = (uint8_t)(data & 0x000000FF);
		data >>= 8;
	}
}

void insertData(uint8_t *target, uint32_t data, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		target[i] = (uint8_t)(data & 0x000000FF);
		data >>= 8;
	}
}

void resolveReferences(
	struct Object *object,
	struct Symbol *symbol,
	size_t symbol_len,
	uint32_t *err_ct
) {
	for (size_t i = 0; i < object->reference_len; ++i) {
		if (object->reference[i].type == REF_RESOLVED) continue;
		for (size_t j = 0; j < symbol_len; ++j) {
			if (strcmp(object->reference[i].name, symbol[j].name) == 0) {
				fillReference(object, &object->reference[i], symbol[j].value, err_ct);
				break;
			}
		}
	}
}

void fillReference(
	struct Object *object,
	struct Reference *reference,
	uint32_t value,
	uint32_t *err_ct
) {
	uint8_t *target = NULL;
	uint32_t data = 0;
	for (size_t i = 0; i < object->segment_len; ++i) {
		if (
			reference->address >= object->segment[i].address &&
			reference->address < object->segment[i].address + object->segment[i].data_len
		) {
			target = &object->segment[i].data[reference->address - object->segment[i].address];
		}
	}
	switch (reference->type) {
		case REF_DATA:
			insertData(target, value, reference->size);
			break;

		case REF_RELATIVE:
			data = value - reference->address - 2;
			if (data > 255) {
				printf(
					"(%s:%u) Branch to '%s' is too far.\n",
					reference->path,
					reference->line,
					reference->name
				);
				++*err_ct;
			}
			else insertData(target, data, 1);
			break;

		case REF_ABSOLUTE:
			data = reference->size == 2 ? normalizeWord(value) : normalizeLong(value);
			insertData(target, data, reference->size);
			break;

		default: break;
	}
	reference->type = REF_RESOLVED;
}