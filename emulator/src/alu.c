#include "alu.h"
#include "control.h"

uint8_t calcALU(uint8_t in_a, uint8_t in_b, enum Flag *flags, enum Control control) {
	uint16_t output = 0;
	enum Flag out_flags = *flags;
	if (!(control & CTRL_EZ)) in_a = 0;
	if (control & CTRL_EN) in_b = ~in_b;
	switch (control & CTRL_EO) {
		case CTRL_ES:
			output = (uint16_t)in_a + (uint16_t)in_b + (control & CTRL_EC ? 1 : 0);
			out_flags &= ~FLAG_C;
			out_flags |= output & 0x0100 ? FLAG_C : 0;
			break;

		case CTRL_ER:
			out_flags &= ~FLAG_C;
			out_flags |= in_b & 0x01 ? FLAG_C : 0;
			output = (in_b >> 1) | ((*flags & CTRL_EC) << 5);
			break;

		case CTRL_EA: output = in_a & in_b; break;
		case CTRL_EO: output = in_a | in_b; break;
	}
	out_flags &= ~FLAG_N & ~FLAG_Z;
	out_flags |= output & 0x0080 ? FLAG_N : 0;
	out_flags |= (output & 0x00FF) == 0 ? FLAG_Z : 0;
	if (!(control & CTRL_FI) && !(control & CTRL_FD)) *flags = out_flags;
	return (uint8_t)(output & 0x00FF);
}