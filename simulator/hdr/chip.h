#ifndef HDR_CHIP
#define HDR_CHIP
#include <stddef.h>
#include "chip-data.h"

struct Net;

struct Circuit;

struct Chip {
	enum ChipType type;
	struct Circuit *circ;
	struct Net **in, **out *local;
	unsigned char *mem;
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