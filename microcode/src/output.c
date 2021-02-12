#include "output.h"
#include "arguments.h"
#include <stdlib.h>
#include <stdio.h>

void outputToFile(uint8_t *output, size_t output_size) {
	FILE *output_file = fopen(getOutputPath(), "r");
	if (!output_file) {
		printf("Unable to open output file '%s'\n", getOutputPath());
		exit(1);
	}

	fclose(output_file);
	free(output);
}
