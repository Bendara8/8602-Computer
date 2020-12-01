#ifndef HDR_CHIP_DATA
#define HDR_CHIP_DATA
#include <stddef.h>

enum ChipType {
	CHIP_74HC00,  /* Quad Nand Gate
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
	CHIP_74HC377, /* 8-bit Register
	 * 3 in
	 * 1 out
	 * 2 local
	 * in0: Data in
	 * in1: ~Enable
	 * in2: Clock
	 * out0: Data out
	 * local0: Data in
	 * local1: ~Enable
	 */
};

struct ChipData {
	size_t in;
	size_t out;
	size_t local;
	size_t mem;
};

const struct ChipData CHIP_DATA[] = {
	{8, 4, 0, 0},
	{3, 1, 2, 0},
};

#endif