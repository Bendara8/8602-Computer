#ifndef HDR_CHIP_DATA
#define HDR_CHIP_DATA
#include <stddef.h>

enum ChipType {
	CHIP_74HC00,  /* Quad 2-input NAND Gate
	 * 8 in, 4 out
	 * in0,1: Gate A inputs
	 * in2,3: Gate B inputs
	 * in4,5: Gate C inputs
	 * in6,7: Gate D inputs
	 * out0: Gate A output
	 * out1: Gate B output
	 * out2: Gate C output
	 * out3: Gate D output
	 */
	CHIP_74HC02,  /* Quad 2-input NOR Gate
	 * 8 in, 4 out
	 * in0,1: Gate A inputs
	 * in2,3: Gate B inputs
	 * in4,5: Gate C inputs
	 * in6,7: Gate D inputs
	 * out0: Gate A output
	 * out1: Gate B output
	 * out2: Gate C output
	 * out3: Gate D output
	 */
	CHIP_74HC08,  /* Quad 2-input AND Gate
	 * 8 in, 4 out
	 * in0,1: Gate A inputs
	 * in2,3: Gate B inputs
	 * in4,5: Gate C inputs
	 * in6,7: Gate D inputs
	 * out0: Gate A output
	 * out1: Gate B output
	 * out2: Gate C output
	 * out3: Gate D output
	 */
	CHIP_74HC32,  /* Quad 2-input OR Gate
	 * 8 in, 4 out
	 * in0,1: Gate A inputs
	 * in2,3: Gate B inputs
	 * in4,5: Gate C inputs
	 * in6,7: Gate D inputs
	 * out0: Gate A output
	 * out1: Gate B output
	 * out2: Gate C output
	 * out3: Gate D output
	 */
	CHIP_74HC86,  /* Quad 2-input XOR Gate
	 * 8 in, 4 out
	 * in0,1: Gate A inputs
	 * in2,3: Gate B inputs
	 * in4,5: Gate C inputs
	 * in6,7: Gate D inputs
	 * out0: Gate A output
	 * out1: Gate B output
	 * out2: Gate C output
	 * out3: Gate D output
	 */
	CHIP_74HC21,  /* Dual 4-input AND Gate
	 * 8 in, 2 out
	 * in0,1,2,3: Gate A inputs
	 * in4,5,6,7: Gate B inputs
	 * out0: Gate A output
	 * out1: Gate B output
	 */
	CHIP_74HC157, /* 4-bit 2-input Multiplexor
	 * 10 in, 4 out
	 * in0-3: Input data 0
	 * in4-7: Input data 1
	 * in8: Select
	 * in9: ~Enable
	 * out0-3: Output data
	 */
	CHIP_74HC153, /* 2-bit 4-input Multiplexor
	 * 12 in, 2 out
	 * in0-1: Input data 0
	 * in2-3: Input data 1
	 * in4-5: Input data 2
	 * in6-7: Input data 3
	 * in8-9: Select
	 * in10-11: ~Enable inputs
	 * out0-1: Output data
	 */
	CHIP_74HC283, /* 4-bit Adder
	 * 9 in, 5 out
	 * in0-3: Input data A
	 * in4-7: Input data B
	 * in8: Carry in
	 * out0-3: Output data
	 * out4: Carry out
	 */
	CHIP_74HC377, /* 8-bit Register
	 * 10 in, 8 out, 9 local
	 * in0-7: Input data
	 * in8: ~Enable
	 * in9: Clock
	 * out0-7: Output data
	 * local0-7: Input data
	 * local8: ~Enable
	 */
};

struct ChipData {
	size_t in;
	size_t out;
	size_t local;
	size_t mem;
};

extern const struct ChipData CHIP_DATA[];

#endif