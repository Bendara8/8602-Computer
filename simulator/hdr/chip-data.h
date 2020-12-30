#ifndef HDR_CHIP_DATA
#define HDR_CHIP_DATA
#include <stddef.h>

enum ChipType {
	CHIP_CLK40M,
	CHIP_74HC00,
	CHIP_74HC30,
	CHIP_74HC02,
	CHIP_74HC04,
	CHIP_74HC08,
	CHIP_74HC21,
	CHIP_74HC32,
	CHIP_74HC86,
	CHIP_74HC157,
	CHIP_74HC153,
	CHIP_74AC161,
	CHIP_74HC283,
	CHIP_74HC377,
};

struct ChipData {
	char *name;
	size_t in;
	size_t out;
	size_t local;
	size_t mem;
};

enum ChipType strToChipType(
	char *str
);

extern const struct ChipData CHIP_DATA[];
extern const size_t CHIP_DATA_LEN;

#endif