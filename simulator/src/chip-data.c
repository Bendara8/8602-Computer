#include <string.h>
#include "chip-data.h"
#include "error.h"

enum ChipType strToChipType(char *str) {
	for (size_t i = 0; i < CHIP_DATA_LEN; ++i) {
		if (strcmp(CHIP_DATA[i].name, str) == 0) return (enum ChipType)i;
	}
	raiseError(ERROR_NOT_CHIP, str);
	raiseAbort(ABORT_FATAL_ERROR);
	return (enum ChipType)-1;
}

const struct ChipData CHIP_DATA[] = {
	{"CLK40M",    1, 2, 0, 0},
	{"_74HC00",   8, 4, 0, 0},
	{"_74HC30",   8, 1, 0, 0},
	{"_74HC02",   8, 4, 0, 0},
	{"_74HC04",   6, 6, 0, 0},
	{"_74HC08",   8, 4, 0, 0},
	{"_74HC21",   8, 2, 0, 0},
	{"_74HC32",   8, 4, 0, 0},
	{"_74HC86",   8, 4, 0, 0},
	{"_74HC157", 10, 4, 0, 0},
	{"_74HC153", 12, 2, 0, 0},
	{"_74AC161",  9, 5, 5, 0},
	{"_74HC283",  9, 5, 0, 0},
	{"_74HC377", 10, 8, 9, 0},
};
const size_t CHIP_DATA_LEN = sizeof CHIP_DATA / sizeof CHIP_DATA[0];