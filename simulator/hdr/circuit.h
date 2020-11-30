#ifndef HDR_CIRCUIT
#define HDR_CIRCUIT

struct Chip;

struct Net;

struct Circuit {
	struct {
		struct Chip *buf;
		size_t len;
	} chip_arr;
	struct {
		struct Net *buf;
		size_t len;
	} net_arr;
};

#endif