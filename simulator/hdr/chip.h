#ifndef HDR_CHIP
#define HDR_CHIP

enum ChipType {
	CHIP_74HC377,
};

enum ChipState {
	STATE_
};

struct Net;

struct Chip {
	enum ChipType type;
	enum ChipState state;
	long delay_ctr;
	struct Net **in_arr, **out_arr;
	union {
		char *mem_arr;
		long val;
	};
};

#endif