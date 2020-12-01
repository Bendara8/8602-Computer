#ifndef HDR_CIRCUIT
#define HDR_CIRCUIT
#include <stddef.h>

struct Chip;

struct Net;

struct NetUpdate;

struct Circuit {
	struct {
		struct Chip *arr;
		size_t len;
	} chip;
	struct {
		struct Net *arr;
		size_t len;
	} net;
	struct {
		struct NetUpdate *head;
	} update, empty;
};

void addNetUpdate(
	struct Circuit *circ,
	struct Net *target,
	int val,
	int delay
);

#endif