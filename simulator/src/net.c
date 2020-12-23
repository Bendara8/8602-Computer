#include <stdlib.h>
#include <stdio.h> // TEMP
#include <string.h>
#include "net.h"
#include "error.h"

struct NetUpdate *allocNetUpdateBlock(size_t len) {
	struct NetUpdate *ret = malloc(sizeof (struct NetUpdate) * len);
	if (!ret) raiseAbort(
		ABORT_ALLOCATION
	);
	addFreeTarget(ret);
	for (size_t i = 0; i < len - 1; ++i) {
		ret[i].next = &ret[i + 1];
		ret[i].target = NULL;
		ret[i].val = 0;
		ret[i].delay = 0;
	}
	ret[len - 1].next = NULL;
	ret[len - 1].target = NULL;
	ret[len - 1].val = 0;
	ret[len - 1].delay = 0;
	return ret;
}

struct NetUpdate **stepNetUpdate(
	struct NetUpdate *update,
	struct NetUpdate **last,
	struct NetUpdate **empty_head
) {
	--update->delay;
	if (update->delay <= 0) {
		if (update->val != 0 && update->val != 1) {
			printf("Net %p out of bounds with val=%i\n", (void *)update->target, update->val);
		}
		if (update->target->val != update->val) update->target->changed = 1;
		update->target->val = update->val;
		*last = update->next;
		update->next = *empty_head;
		*empty_head = update;
		update->target = NULL;
		update->val = 0;
		update->delay = 0;
		return last;
	}
	else return &update->next;
}

void initNetVec(struct NetVec *vec, size_t cap) {
	vec->buf = malloc(sizeof (struct Net) * cap);
	if (!vec->buf) raiseAbort(
		ABORT_ALLOCATION
	);
	addFreeTarget(vec->buf);
	vec->cap = cap;
	vec->len = 0;
}

void initBusVec(struct BusVec *vec, size_t cap) {
	vec->buf = malloc(sizeof (struct Bus) * cap);
	if (!vec->buf) raiseAbort(
		ABORT_ALLOCATION
	);
	addFreeTarget(vec->buf);
	vec->cap = cap;
	vec->len = 0;
}

struct Net *addNet(struct NetVec *vec, char *name) {
	if (vec->len == vec->cap) {
		vec->cap *= 2;
		void **target = findFreeTarget(vec->buf);
		if (!target) raiseAbort(ABORT_NO_FREE_TARGET);
		vec->buf = realloc(vec->buf, sizeof (struct Net) * vec->cap);
		if (!vec->buf) raiseAbort(
			ABORT_ALLOCATION
		);
		*target = vec->buf;
	}
	vec->buf[vec->len].name = name;
	vec->buf[vec->len].val = 0;
	vec->buf[vec->len].changed = 1;
	return &vec->buf[vec->len++];
}

struct Net *findNet(struct NetVec *vec, char *name) {
	for (size_t i = 0; i < vec->len; ++i) {
		if (vec->buf[i].name && strcmp(vec->buf[i].name, name) == 0) return &vec->buf[i];
	}
	return NULL;
}

struct Bus *addBus(struct BusVec *vec, struct NetVec *net_vec, char *name, size_t width) {
	if (vec->len == vec->cap) {
		vec->cap *= 2;
		void **target = findFreeTarget(vec->buf);
		if (!target) raiseAbort(ABORT_NO_FREE_TARGET);
		vec->buf = realloc(vec->buf, sizeof (struct Bus) * vec->cap);
		if (!vec->buf) raiseAbort(
			ABORT_ALLOCATION
		);
		*target = vec->buf;
	}
	vec->buf[vec->len].name = name;
	vec->buf[vec->len].width = width;
	vec->buf[vec->len].net_idx = net_vec->len;
	for (size_t i = 0; i < width; ++i) addNet(net_vec, NULL);
	return &vec->buf[vec->len++];
}

struct Bus *findBus(struct BusVec *vec, char *name) {
	for (size_t i = 0; i < vec->len; ++i) {
		if (strcmp(vec->buf[i].name, name) == 0) return &vec->buf[i];
	}
	return NULL;
}

struct Bus *findBusFromNetIdx(struct BusVec *vec, size_t net_idx) {
	for (size_t i = 0; i < vec->len; ++i) {
		if (net_idx - vec->buf[i].net_idx < vec->buf[i].width) return &vec->buf[i];
	}
	return NULL;
}

struct NetUpdate *findNetUpdateFromNet(struct NetUpdate *head, struct Net *net) {
	struct NetUpdate *curr = head;
	struct NetUpdate *min = NULL;
	while (curr) {
		if (curr->target == net && (!min || curr->delay < min->delay)) min = curr; 
		curr = curr->next;
	}
	return min;
}

int busToInt(struct Bus *bus, struct NetVec *net_vec) {
	int result = (int)busToUns(bus, net_vec);
	if (result < (1 << (bus->width - 1))) return result;
	else return result | (int)((unsigned)~0 << bus->width);
}

unsigned busToUns(struct Bus *bus, struct NetVec *net_vec) {
	unsigned result = 0;
	for (size_t i = bus->width - 1; i < bus->width; --i) {
		result <<= 1;
		result |= net_vec->buf[bus->net_idx + i].val;
	}
	return result;
}