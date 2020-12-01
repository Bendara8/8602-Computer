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
	int delay;
	int val;
};

#endif