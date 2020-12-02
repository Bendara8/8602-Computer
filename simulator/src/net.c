#include <stdlib.h>
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
		update->target->val = update->val;
		update->target->changed = 1;
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