#include "linker.h"
#include "symbol.h"
#include "object.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static bool intersecting(struct Segment *a, struct Segment *b);

static size_t OUTPUT_SIZE = 262144;

static struct Object *object = NULL;
static size_t object_len = 0;
static uint32_t err_ct = 0;
static uint8_t *output = NULL;

void linkObjectsIntoBinary(char *path) {
	object = getObjectList();
	object_len = getObjectLen();
	for (size_t i = 0; i < object_len; ++i) {
		for (size_t j = 0; j < object[i].segment_len - 1; ++j) {
			for (size_t k = i; k < object_len; ++k) {
				for (size_t l = j + 1; l < object[k].segment_len; ++l) {
					if (intersecting(&object[i].segment[j], &object[k].segment[l])) {
						printf(
							"(%s:%u) Segment intersects with segment at (%s:%u).\n",
							object[i].path,
							object[i].segment[j].line,
							object[k].path,
							object[k].segment[l].line
						);
						++err_ct;
					}
				}
			}
		}
	}
	for (size_t i = 0; i < object_len; ++i) {
		for (size_t j = 0; j < object_len; ++j) {
			if (i == j) continue;
			resolveReferences(&object[i], object[j].symbol, object[j].symbol_len, &err_ct);
		}
		for (size_t j = 0; j < object[i].reference_len; ++j) {
			if (object[i].reference[j].type != REF_RESOLVED) {
				printf(
					"(%s:%u) Unable to resolve reference '%s'.\n",
					object[i].path,
					object[i].reference[j].line,
					object[i].reference[j].name
				);
				++err_ct;
			}
		}
	}
	if (err_ct > 0) {
		printf("Linker had %u errors.\n", err_ct);
		exit(EXIT_FAILURE);
	}
	output = malloc(OUTPUT_SIZE);
	if (output == NULL) {
		puts("Unable to allocate output buffer.");
		exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < object_len; ++i) {
		for (size_t j = 0; j < object[i].segment_len; ++j) {
			struct Segment *segment = &object[i].segment[j];
			memcpy(&output[segment->address], segment->data, segment->data_len);
		}
	}
	FILE *file = fopen(path, "wb");
	if (file == NULL) {
		printf("Unable to open output file '%s'.", path);
		free(output);
		exit(EXIT_FAILURE);
	}
	fwrite(output, 1, OUTPUT_SIZE, file);
	free(output);
	fclose(file);
}

bool intersecting(struct Segment *a, struct Segment *b) {
	if (a->address < b->address) {
		if (a->address + a->data_len > b->address) return true;
	}
	else if (b->address < a->address) {
		if (b->address + b->data_len > a->address) return true;
	}
	else return true;
	return false;
}