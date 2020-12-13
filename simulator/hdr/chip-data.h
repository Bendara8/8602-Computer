#ifndef HDR_CHIP_DATA
#define HDR_CHIP_DATA
#include <stddef.h>

enum ChipType {
	CHIP_74HC00,  /* Quad 2-input NAND Gate
	 * 8 in, 4 out
	 * in:0,1 = Gate A inputs
	 * in:2,3 = Gate B inputs
	 * in:4,5 = Gate C inputs
	 * in:6,7 = Gate D inputs
	 * out:0 = Gate A output
	 * out:1 = Gate B output
	 * out:2 = Gate C output
	 * out:3 = Gate D output
	 */
	CHIP_74HC02,  /* Quad 2-input NOR Gate
	 * 8 in, 4 out
	 * in:0,1 = Gate A inputs
	 * in:2,3 = Gate B inputs
	 * in:4,5 = Gate C inputs
	 * in:6,7 = Gate D inputs
	 * out:0 = Gate A output
	 * out:1 = Gate B output
	 * out:2 = Gate C output
	 * out:3 = Gate D output
	 */
	CHIP_74HC08,  /* Quad 2-input AND Gate
	 * 8 in, 4 out
	 * in:0,1 = Gate A inputs
	 * in:2,3 = Gate B inputs
	 * in:4,5 = Gate C inputs
	 * in:6,7 = Gate D inputs
	 * out:0 = Gate A output
	 * out:1 = Gate B output
	 * out:2 = Gate C output
	 * out:3 = Gate D output
	 */
	CHIP_74HC32,  /* Quad 2-input OR Gate
	 * 8 in, 4 out
	 * in:0,1 = Gate A inputs
	 * in:2,3 = Gate B inputs
	 * in:4,5 = Gate C inputs
	 * in:6,7 = Gate D inputs
	 * out:0 = Gate A output
	 * out:1 = Gate B output
	 * out:2 = Gate C output
	 * out:3 = Gate D output
	 */
	CHIP_74HC86,  /* Quad 2-input XOR Gate
	 * 8 in, 4 out
	 * in:0,1 = Gate A inputs
	 * in:2,3 = Gate B inputs
	 * in:4,5 = Gate C inputs
	 * in:6,7 = Gate D inputs
	 * out:0 = Gate A output
	 * out:1 = Gate B output
	 * out:2 = Gate C output
	 * out:3 = Gate D output
	 */
	CHIP_74HC21,  /* Dual 4-input AND Gate
	 * 8 in, 2 out
	 * in:0,1,2,3 = Gate A inputs
	 * in:4,5,6,7 = Gate B inputs
	 * out:0 = Gate A output
	 * out:1 = Gate B output
	 */
	CHIP_74HC157, /* 4-bit 2-input Multiplexor
	 * 10 in, 4 out
	 * in:0-3 = Input data 0
	 * in:4-7 = Input data 1
	 * in:8 = Select
	 * in:9 = ~Enable
	 * out0-3: Output data
	 */
	CHIP_74HC153, /* 2-bit 4-input Multiplexor
	 * 12 in, 2 out
	 * in:0-1 = Input data 0
	 * in:2-3 = Input data 1
	 * in:4-5 = Input data 2
	 * in:6-7 = Input data 3
	 * in:8-9 = Select
	 * in:10-11 = ~Enable inputs
	 * out:0-1 = Output data
	 */
	CHIP_74HC283, /* 4-bit Adder
	 * 9 in, 5 out
	 * in:0-3 = Input data A
	 * in:4-7 = Input data B
	 * in:8 = Carry in
	 * out:0-3 = Output data
	 * out:4 = Carry out
	 */
	CHIP_74HC377, /* 8-bit Register
	 * 10 in, 8 out, 9 local
	 * in:0-7 = Input data
	 * in:8 = ~Enable
	 * in:9 = Clock
	 * out:0-7 = Output data
	 * local:0-7 = Input data
	 * local:8 = ~Enable
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