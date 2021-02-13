#include "output.h"
#include "arguments.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void multiFile(uint8_t *output, size_t output_size, char *hash);
static void singleFile(uint8_t *output, size_t output_size);
static FILE *openFile(char *path);

void outputToFile(uint8_t *output, size_t output_size) {
	char *hash = strchr(getOutputPath(), '#');
	if (hash != NULL) {
		multiFile(output, output_size, hash);
	}
	else {
		singleFile(output, output_size);
	}
	free(output);
}

void multiFile(uint8_t *output, size_t output_size, char *hash) {
	FILE *output_file[3] = {NULL};
	for (size_t i = 0; i < 3; ++i) {
		*hash = i + '0';
		output_file[i] = openFile(getOutputPath());
		if (!output_file[i]) {
			for (size_t j = i - 1; j < i; --j) {
				fclose(output_file[j]);
			}
			return;
		}
	}
	for (size_t i = 0; i < 3; ++i) {
		fwrite(output + (i * (output_size / 3)), sizeof output[0], output_size / 3, output_file[i]);
		fclose(output_file[i]);
	}
}

void singleFile(uint8_t *output, size_t output_size) {
	FILE *output_file = openFile(getOutputPath());
	if (!output_file) return;
	fwrite(output, sizeof output[0], output_size, output_file);
	fclose(output_file);
}

FILE *openFile(char *path) {
	FILE *file = fopen(path, "wb");
	if (!file) {
		printf("Unable to open output file '%s'\n", path);
		return NULL;
	}
	return file;
}