#ifndef HDR_CIRCUIT
#define HDR_CIRCUIT

struct Chip;

struct Net;

struct Circuit {
	struct Chip *chip_arr;
	struct Net *net_arr;
};

#endif