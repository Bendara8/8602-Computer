#ifndef HDR_CHIP
#define HDR_CHIP
#include <stddef.h>

enum ChipType {
	CHIP_74HC00,
	CHIP_74HC377,
};

enum ChipState {
	STATE_STABLE,
	STATE_PROP_DELAY,
};

struct Net;

struct Chip {
	enum ChipType type;
	enum ChipState state;
	long delay_ctr;
	struct {
		struct Net **buf;
		size_t len;
	} in_arr, out_arr;
	union {
		struct {
			unsigned char *buf;
			size_t len;
		} mem_arr;
		long val;
	};
};

void initChip(
	struct Chip *chip,
	enum ChipType type,
	size_t in_len,
	size_t out_len
);

void initChipMemory(
	struct Chip *chip,
	size_t len
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