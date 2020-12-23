#ifndef HDR_CHIP
#define HDR_CHIP
#include <stddef.h>
#include "chip-data.h"

struct Net;

struct Circuit;

struct Chip {
	enum ChipType type;
	char *name;
	struct Circuit *circ;
	struct Net **in, **out, *local;
	unsigned char *mem;
};

struct ChipVec {
	struct Chip *buf;
	size_t len, cap;
};

void initChip(
	struct Chip *chip,
	enum ChipType type,
	char *name,
	struct Circuit *circ
);

void initChipVec(
	struct ChipVec *vec,
	size_t cap
);

struct Chip *addChip(
	struct ChipVec *vec,
	char *type_str,
	char *name,
	struct Circuit *circ
);

struct Chip *findChip(
	struct ChipVec *vec,
	char *name
);

void stepChip(
	struct Chip *chip
);

void step74HC00(
	struct Chip *chip
);

void step74HC02(
	struct Chip *chip
);

void step74HC08(
	struct Chip *chip
);

void step74HC32(
	struct Chip *chip
);

void step74HC86(
	struct Chip *chip
);

void step74HC21(
	struct Chip *chip
);

void step74HC157(
	struct Chip *chip
);

void step74HC153(
	struct Chip *chip
);

void step74HC283(
	struct Chip *chip
);

void step74HC377(
	struct Chip *chip
);

#endif