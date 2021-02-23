#include "lexer.h"
#include "token.h"
#include <stdio.h>

void lexFileIntoTokens(char *path) {
	FILE *file = fopen(path, "r");
	if (file == NULL) {
		printf("Unable to open target file '%s'\n", path);
		exit(EXIT_FAILURE);
	}

	resetTokenQueue();

	int chr = getc(file);
	while (chr != EOF) {
		switch (chr) {
			
		}
	}
}