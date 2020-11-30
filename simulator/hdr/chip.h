#ifndef HDR_CHIP
#define HDR_CHIP
#include <stddef.h>

enum ChipType {
	CHIP_74HC00, /*
	 * Quad Nand Gate
	 * 8 in
	 * 4 out
	 * in0,1: Gate A inputs
	 * in2,3: Gate B inputs
	 * in4,5: Gate C inputs
	 * in6,7: Gate D inputs
	 * out0: Gate A output
	 * out1: Gate B output
	 * out2: Gate C output
	 * out3: Gate D output
	 */
	CHIP_74HC377, /*
	 * 8-bit Register
	 * 3 in
	 * 1 out
	 * in0: Data in
	 * in1: ~Enable
	 * in2: Clock
	 * out0: Data out
	 */
};

enum ChipState {
	STATE_STABLE,
	STATE_PROP_DELAY,
};

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