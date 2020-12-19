#ifndef HDR_INTERFACE
#define HDR_INTERFACE
#include <stddef.h>

enum Command {
	CMD_NONE,
	CMD_NANO,
	CMD_CLOCK,
	CMD_STEP,
	CMD_RUN,
	CMD_QUIT,
	CMD_UPDATES,
	CMD_CHIP,
	CMD_BUS,
	CMD_NET,
	CMD_HEX,
	CMD_SET,
	CMD_TOUCH,
};

struct Circuit;

void printInfo(
	void
);

void enterInterface(
	struct Circuit *circ
);

#endif