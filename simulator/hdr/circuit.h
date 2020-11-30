#ifndef HDR_CIRCUIT
#define HDR_CIRCUIT

struct Chip;

struct Net;

struct Circuit {
	struct {
		struct Chip *arr;
		size_t len;
	} chip;
	struct {
		struct Net *arr;
		size_t len;
	} net;
};

#endif