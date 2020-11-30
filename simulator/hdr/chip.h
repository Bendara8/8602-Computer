#ifndef HDR_CHIP
#define HDR_CHIP
#include <stddef.h>
#include "chip-data.h"

struct Net;

struct Chip {
	enum ChipType type;
	enum ChipState state;
	long delay;
	struct {
		struct Net **arr;
		size_t len;
	} in, out;
	struct {
		unsigned char *arr;
		size_t len;
	} mem;
	int val;
};

void initChip(
	struct Chip *chip,
	enum ChipType type
);

void stepChip(
	struct Chip *chip
);

void step74HC00(
	struct Chip *chip
);

void step74HC377(
	struct Chip *chip
);

#endif