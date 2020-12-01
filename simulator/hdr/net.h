#ifndef HDR_NET
#define HDR_NET
#include <stddef.h>

struct Net {
	int val;
	int changed;
};

struct NetUpdate {
	struct NetUpdate *next;
	struct Net *target;
	int val;
	int delay;
};

struct NetUpdate *allocNetUpdateBlock(
	size_t len
);

#endif