#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "interface.h"
#include "circuit.h"
#include "chip.h"
#include "net.h"
#include "lexer.h"
#include "error.h"

static const char *CMD_STR[] = {
	"",
	"nano",
	"clock",
	"run",
	"quit",
	"updates",
	"chip",
	"bus",
	"net",
	"hex",
	"set",
};
static const size_t CMD_STR_LEN = sizeof CMD_STR / sizeof CMD_STR[0];

static enum Command strToCommand(char *str) {
	for (size_t i = 0; i < CMD_STR_LEN; ++i) {
		if (strcmp(str, CMD_STR[i]) == 0) return (enum Command)i;
	}
	return CMD_NONE;
}

void printInfo(void) {
	printf("--- 8602 Simulator ---\n");
}

static void printStatus(struct Circuit *circ, size_t num_nets_updated) {
	printf(
		"%lu ns elapsed, %zu nets updated, %zu future net updates\n",
		getElapsedTime(),
		num_nets_updated,
		lenSchedule(circ)
	);
}

static void printNetUpdate(struct NetUpdate *update, struct Circuit *circ) {
	if (update->target->name) printf(
		"%s (%i in %i ns)\n",
		update->target->name,
		update->val,
		update->delay
	);
	else {
		size_t net_idx = update->target - circ->net_vec.buf;
		struct Bus *bus = findBusFromNetIdx(&circ->bus_vec, net_idx);
		if (bus) printf(
			"%s:%zu (%i in %i ns)\n",
			bus->name,
			net_idx - bus->net_idx,
			update->val,
			update->delay
		);
	}
}

static void printNetStatus(struct Net *net, struct Circuit *circ) {
	printf(" = %i", net->val);
	struct NetUpdate *update = findNetUpdateFromNet(circ->update_list.head, net);
	if (update) printf(" (%i in %i ns)", update->val, update->delay);
	printf("\n");
}

static void printNet(struct Net *net, struct Circuit *circ) {
	if (net->name) printf("%s", net->name);
	else {
		size_t net_idx = net - circ->net_vec.buf;
		struct Bus *bus = findBusFromNetIdx(&circ->bus_vec, net_idx);
		if (bus) printf("%s:%zu", bus->name, net_idx - bus->net_idx);
	}
	printNetStatus(net, circ);
}

static void printBus(struct Bus *bus, struct Circuit *circ) {
	int bus_int = busToInt(bus, &circ->net_vec);
	unsigned bus_uns = busToUns(bus, &circ->net_vec);
	printf("%s = %i +%u $%0*X\n", bus->name, bus_int, bus_uns, (int)((bus->width + 3) / 4), bus_uns);
	for (size_t i = 0; i < bus->width; ++i) {
		struct Net *net = &circ->net_vec.buf[bus->net_idx + i];
		printf("%s:%zu = %i", bus->name, i, net->val);
		struct NetUpdate *update = findNetUpdateFromNet(circ->update_list.head, net);
		if (update) printf(" (%i in %i ns)", update->val, update->delay);
		printf("\n");
	}
}

static void printPins(
	char *group,
	struct Net **net_ptr_arr,
	size_t net_ptr_arr_len,
	struct Circuit *circ
) {
	for (size_t i = 0; i < net_ptr_arr_len; ++i) {
		printf("%s:%zu = ", group, i);
		if (net_ptr_arr[i]) printNet(net_ptr_arr[i], circ);
		else printf("null\n");
	}
}

static void printLocals(struct Net *net_arr, size_t net_arr_len, struct Circuit *circ) {
	for (size_t i = 0; i < net_arr_len; ++i) {
		printf("local:%zu = %i", i, net_arr[i].val);
		struct NetUpdate *update = findNetUpdateFromNet(circ->update_list.head, &net_arr[i]);
		if (update) printf(" (%i in %i ns)", update->val, update->delay);
		printf("\n");
	}
}

static void printChip(struct Chip *chip) {
	struct ChipData data = CHIP_DATA[chip->type];
	printf("%s\n", data.name);
	if (data.mem > 0) printf("%zu bytes of onboard memory\n", data.mem);
	printPins("in", chip->in, data.in, chip->circ);
	printPins("out", chip->out, data.out, chip->circ);
	printLocals(chip->local, data.local, chip->circ);
}

static void printHex(struct Chip *chip, size_t addr_width, size_t start, size_t size) {
	for (size_t row = start; row < start + size; row += 16) {
		printf("$%0*lX: ", (int)((addr_width + 3) / 4), row);
		for (size_t col = row; col < row + 16; ++col) {
			printf("%02hhX ", chip->mem[col]);
		}
		printf("|");
		char disp, data;
		for (size_t col = row; col < row + 16; ++col) {
			data = chip->mem[col];
			if (isprint(data)) disp = data;
			else disp = '.';
			printf("%c", disp);
		}
		printf("|\n");
	}
}

static void splitInput(char *input_str, char **command, size_t command_len) {
	size_t i = 0, j = 0;
	char prev = ' ';
	while (input_str[i] != '\0') {
		if (isspace(prev) && !isspace(input_str[i])) {
			if (j < command_len) {
				command[j] = &input_str[i];
				++j;
			}
			else {
				raiseError(ERROR_LONG_COMMAND, command_len);
				j = 0;
				break;
			}
			prev = input_str[i];
		}
		else if (!isspace(prev) && isspace(input_str[i])) {
			prev = input_str[i];
			input_str[i] = '\0';
		}
		else prev = input_str[i];
		++i;
	}
	for (i = j; i < command_len; ++i) {
		command[i] = NULL;
	}
}

static size_t numCommandArgs(char **command, size_t max) {
	for (size_t i = 0; i <= max; ++i) {
		if (!command[i + 1]) return i;
	}
	return max + 1;
}

static void setBus(char **command, struct Circuit *circ) {
	struct Bus *bus = findBus(&circ->bus_vec, command[2]);
	if (!bus) {
		raiseError(ERROR_NOT_BUS, command[2]);
		return;
	}
	unsigned num = parseNum(command[3]);
	if (getErrorFlag()) return;
	if (num >= (unsigned)(1 << bus->width)) {
		raiseError(ERROR_INVALID_NUM, (int)num);
		return;
	}
	for (size_t i = 0; i < bus->width; ++i) {
		int val = (int)!!(num & (1 << i));
		struct Net *net = &circ->net_vec.buf[bus->net_idx + i];
		if (net->val != val) {
			net->val = val;
			net->changed = 1;
		}
	}
	printBus(bus, circ);
}

static void setNet(char **command, struct Circuit *circ) {
	char *bus_idx_str = strchr(command[2], ':');
	if (bus_idx_str) {
		bus_idx_str[0] = '\0';
		++bus_idx_str;
		struct Bus *bus = findBus(&circ->bus_vec, command[2]);
		if (bus) {
			size_t bus_idx = parseNum(bus_idx_str);
			if (getErrorFlag()) return;
			if (bus_idx >= bus->width) raiseError(ERROR_INVALID_NUM, bus_idx);
			else {
				int val = (int)parseNum(command[3]);
				if (getErrorFlag()) return;
				struct Net *net = &circ->net_vec.buf[bus->net_idx + bus_idx];
				if (val == 0 || val == 1) {
					if (net->val != val) {
						net->val = val;
						net->changed = 1;
					}
					printf("%s:%zu", bus->name, bus_idx);
					printNetStatus(&circ->net_vec.buf[bus->net_idx + bus_idx], circ);
				}
				else raiseError(ERROR_INVALID_NUM, val);
			}
		}
		else raiseError(ERROR_NOT_BUS, command[1]);
	}
	else {
		struct Net *net = findNet(&circ->net_vec, command[2]);
		if (net) {
			int val = (int)parseNum(command[3]);
			if (getErrorFlag()) return;
			if (val == 0 || val == 1) {
				if (net->val != val) {
					net->val = val;
					net->changed = 1;
				}
				printf("%s", net->name);
				printNetStatus(net, circ);
			}
			else raiseError(ERROR_INVALID_NUM, val);
		}
		else raiseError(ERROR_NOT_NET, command[2]);
	}
}

static void runCommandNano(char **command, struct Circuit *circ) {
	if (numCommandArgs(command, 1) > 1) {
		raiseError(ERROR_NUM_COMMAND_ARGS, command[0]);
		return;
	}
	unsigned count = 1;
	if (command[1]) {
		count = parseNum(command[1]);
	}
	if (getErrorFlag()) return;
	size_t num_nets_updated = 0;
	for (unsigned i = 0; i < count; ++i) {
		stepCircuit(circ);
		num_nets_updated += numNetsUpdated(circ);
	}
	printStatus(circ, num_nets_updated);
}

static void runCommandClock(char **command, struct Circuit *circ) {
	command = command; circ = circ;
}

static void runCommandRun(char **command, struct Circuit *circ) {
	command = command; circ = circ;
}

static void runCommandQuit(char **command) {
	if (numCommandArgs(command, 0) != 0) {
		raiseError(ERROR_NUM_COMMAND_ARGS, command[0]);
		return;
	}
	else exit(0);
}

static void runCommandUpdates(char **command, struct Circuit *circ) {
	if (numCommandArgs(command, 0) != 0) {
		raiseError(ERROR_NUM_COMMAND_ARGS, command[0]);
		return;
	}
	struct NetUpdate *curr = circ->update_list.head;
	while (curr) {
		printNetUpdate(curr, circ);
		curr = curr->next;
	}
}

static void runCommandChip(char **command, struct Circuit *circ) {
	if (numCommandArgs(command, 1) != 1) {
		raiseError(ERROR_NUM_COMMAND_ARGS, command[0]);
		return;
	}
	struct Chip *chip = findChip(&circ->chip_vec, command[1]);
	if (chip) printChip(chip);
	else raiseError(ERROR_NOT_CHIP, command[1]);
}

static void runCommandBus(char **command, struct Circuit *circ) {
	if (numCommandArgs(command, 1) != 1) {
		raiseError(ERROR_NUM_COMMAND_ARGS, command[0]);
		return;
	}
	struct Bus *bus = findBus(&circ->bus_vec, command[1]);
	if (bus) printBus(bus, circ);
	else raiseError(ERROR_NOT_BUS, command[1]);
}

static void runCommandNet(char **command, struct Circuit *circ) {
	if (numCommandArgs(command, 1) != 1) {
		raiseError(ERROR_NUM_COMMAND_ARGS, command[0]);
		return;
	}
	char *bus_idx_str = strchr(command[1], ':');
	if (bus_idx_str) {
		bus_idx_str[0] = '\0';
		++bus_idx_str;
		struct Bus *bus = findBus(&circ->bus_vec, command[1]);
		if (bus) {
			size_t bus_idx = parseNum(bus_idx_str);
			if (getErrorFlag()) return;
			if (bus_idx >= bus->width) raiseError(ERROR_INVALID_NUM, bus_idx);
			else {
				printf("%s:%zu", bus->name, bus_idx);
				printNetStatus(&circ->net_vec.buf[bus->net_idx + bus_idx], circ);
			}
		}
		else raiseError(ERROR_NOT_BUS, command[1]);
	}
	else {
		struct Net *net = findNet(&circ->net_vec, command[1]);
		if (net) {
			printf("%s", net->name);
			printNetStatus(net, circ);
		}
		else raiseError(ERROR_NOT_NET, command[1]);
	}
}

static void runCommandHex(char **command, struct Circuit *circ) {
	size_t num_cmd_args = numCommandArgs(command, 3);
	size_t start, size = 64;
	if (num_cmd_args == 3) {
		size = parseNum(command[3]);
		if (getErrorFlag()) return;
	}
	else if (num_cmd_args != 2) {
		raiseError(ERROR_NUM_COMMAND_ARGS, command[0]);
		return;
	}
	start = parseHex(command[2]);
	if (getErrorFlag()) return;
	struct Chip *chip = findChip(&circ->chip_vec, command[1]);
	if (!chip) {
		raiseError(ERROR_NOT_CHIP, command[1]);
		return;
	}
	size_t addr_width = CHIP_DATA[chip->type].mem;
	if (addr_width) printHex(chip, addr_width, start, size);
	else raiseError(ERROR_NO_MEM, chip->name);
}

static void runCommandSet(char **command, struct Circuit *circ) {
	if (numCommandArgs(command, 3) != 3) {
		raiseError(ERROR_NUM_COMMAND_ARGS, command[0]);
		return;
	}
	switch (strToCommand(command[1])) {
		case CMD_BUS:
			setBus(command, circ);
			break;
		case CMD_NET:
			setNet(command, circ);
			break;
		default:
			raiseError(ERROR_INVALID_OPTION, command[1]);
			break;
	}
}

void enterInterface(struct Circuit *circ) {
	char input_str[64];
	char *command[8];
	setErrorLine(0);
	while (1) {
		setErrorFlag(0);
		printf("> ");
		fgets(input_str, sizeof input_str, stdin);
		input_str[strcspn(input_str, "\n")] = '\0';
		splitInput(input_str, command, sizeof command / sizeof command[0]);
		if (command[0]) switch (strToCommand(command[0])) {
			case CMD_NONE:    raiseError(ERROR_NOT_COMMAND, command[0]); break;
			case CMD_NANO:    runCommandNano(command, circ); break;
			case CMD_CLOCK:   runCommandClock(command, circ); break;
			case CMD_RUN:     runCommandRun(command, circ); break;
			case CMD_QUIT:    runCommandQuit(command); break;
			case CMD_UPDATES: runCommandUpdates(command, circ); break;
			case CMD_CHIP:    runCommandChip(command, circ); break;
			case CMD_BUS:     runCommandBus(command, circ); break;
			case CMD_NET:     runCommandNet(command, circ); break;
			case CMD_HEX:     runCommandHex(command, circ); break;
			case CMD_SET:     runCommandSet(command, circ); break;
		}
		printf("\n");
	}
}