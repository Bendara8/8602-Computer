#include "circuit.h"
#include "net.h"
#include "error.h"

void addNetUpdate(struct Circuit *circ, struct Net *target, int val, int delay) {
	if (!circ->empty.head) circ->empty.head = allocNetUpdateBlock(256);
	struct NetUpdate *temp = circ->empty.head->next;
	circ->empty.head->next = circ->update.head;
	circ->update.head = circ->empty.head;
	circ->empty.head = temp;
	circ->update.head->target = target;
	circ->update.head->val = val;
	circ->update.head->delay = delay;
}