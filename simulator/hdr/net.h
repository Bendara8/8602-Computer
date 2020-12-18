#ifndef HDR_NET
#define HDR_NET
#include <stddef.h>

struct Net {
	char *name;
	int val;
	int changed;
};

struct NetVec {
	struct Net *buf;
	size_t len, cap;
};

struct Bus {
	char *name;
	size_t width;
	size_t net_idx;
};

struct BusVec {
	struct Bus *buf;
	size_t len, cap;
};

struct NetUpdate {
	struct NetUpdate *next;
	struct Net *target;
	int val;
	int delay;
};

struct NetUpdateList {
	struct NetUpdate *head;
};

struct NetUpdate *allocNetUpdateBlock(
	size_t len
);

struct NetUpdate **stepNetUpdate(
	struct NetUpdate *update,
	struct NetUpdate **last,
	struct NetUpdate **empty_head
);

void initNetVec(
	struct NetVec *vec,
	size_t cap
);

void initBusVec(
	struct BusVec *vec,
	size_t cap
);

struct Net *addNet(
	struct NetVec *vec,
	char *name
);

struct Net *findNet(
	struct NetVec *vec,
	char *name
);

struct Bus *addBus(
	struct BusVec *vec,
	struct NetVec *net_vec,
	char *name,
	size_t width
);

struct Bus *findBus(
	struct BusVec *vec,
	char *name
);

#endif