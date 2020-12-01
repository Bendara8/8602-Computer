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