#ifndef HDR_CIRCUIT
#define HDR_CIRCUIT
#include <stddef.h>
#include <chip.h>
#include <net.h>

struct Circuit {
	struct ChipVec chip_vec;
	struct BusVec bus_vec;
	struct NetVec net_vec;
	struct NetUpdateList update_list;
	struct NetUpdateList empty_list;
};

void initCircuit(
	struct Circuit *circ,
	char *path
);

void stepCircuit(
	struct Circuit *circ
);

unsigned long getElapsedTime(
	void
);

void addNetUpdate(
	struct Circuit *circ,
	struct Net *target,
	int val,
	int delay
);

size_t numNetsUpdated(
	struct Circuit *circ
);

size_t lenSchedule(
	struct Circuit *circ
);

#endif