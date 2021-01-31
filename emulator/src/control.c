#include "control.h"
#include "memory.h"

enum Control readControl(
	uint8_t step,
	uint8_t flags,
	uint8_t opcode,
	bool interrupt_0,
	bool interrupt_1,
	bool reset
) {
	uint32_t control_in =
		(step << 0) |
		(flags << 4) |
		(opcode << 8) |
		(interrupt_0 << 16) |
		(interrupt_1 << 17) |
		(reset << 18);

	enum Control control_out =
		((enum Control)readMicrocode((0 << 19) | control_in) << 0) |
		((enum Control)readMicrocode((1 << 19) | control_in) << 8) |
		((enum Control)readMicrocode((2 << 19) | control_in) << 16);

	return control_out;
}