/* Notes
 * Use seperate buffer for HO, no pull up resistors
 * Tie enable pin for data bus ouput to 2.5 MHz clock, that way there is no chance of two buffers outputting at one time
 * Swap ORs back to NANDs -> two extra to use in interrupt and I/O circuit
 * Use 4 NANDs for clock combination and inverting
 * Allow for multicolor text by using a color register which holds foreground and background color, pixel bit selects between the two
 * Use same pulse that latches pixel data from VRAM to latch resultant color from last pixel
 * chains of lod A # sto A P+#0 to adjust text mode foreground/background for syntax highlighting, use nop for timing/spacing
 * select y counter line to int with io reg
 * get zero by zeroing bank output and putting that on bus (KZ KO)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "circuit.h"
#include "chip.h"
#include "net.h"
#include "interface.h"
#include "keyboard.h"
#include "display.h"
#include "error.h"

void printNetUpdateStatus(struct Circuit *circ);
size_t netUpdateLen(struct NetUpdate *head);
size_t netChangedCount(struct Net *arr, size_t len);
void printCircuit(struct Circuit *circ);
int busToInt(struct Net *arr, size_t idx, size_t len);

int main() {
	initFreeTargetVec(256);
	struct Circuit circ;
	buildCircuit(&circ);
	initCircuit(&circ);
	printNetUpdateStatus(&circ);
	while (1) {
		char input[32];
		printf("\n> ");
		fgets(input, 32, stdin);
		input[strcspn(input, "\n")] = '\0';
		if (strcmp(input, "quit") == 0) exit(0);
		else if (strcmp(input, "print") == 0) {
			printCircuit(&circ);
		}
		else if (strcmp(input, "set") == 0) {
			printf("net: ");
			fgets(input, 32, stdin);
			input[strcspn(input, "\n")] = '\0';
			size_t net = strtoul(input, NULL, 10);
			printf("val: ");
			fgets(input, 32, stdin);
			input[strcspn(input, "\n")] = '\0';
			int val = strtol(input, NULL, 10);
			if (circ.net.arr[net].val != val) circ.net.arr[net].changed = 1;
			circ.net.arr[net].val = val;
		}
		else if (strcmp(input, "get") == 0) {
			printf("net: ");
			fgets(input, 32, stdin);
			input[strcspn(input, "\n")] = '\0';
			size_t net = strtoul(input, NULL, 10);
			printf("val:%i\n", circ.net.arr[net].val);
		}
		else {
			size_t step_count = strtoul(input, NULL, 10);
			for (size_t i = 0; i < step_count; ++i) {
				stepCircuit(&circ);
			}
			printNetUpdateStatus(&circ);
		}
	}
}

void printNetUpdateStatus(struct Circuit *circ) {
	static size_t last_update_len = 0, last_empty_len = 0;
	size_t update_len = netUpdateLen(circ->update.head);
	size_t empty_len = netUpdateLen(circ->empty.head);
	printf(
		"%zu updates schedhuled, was %zu, delta %i.\n",
		update_len,
		last_update_len,
		(int)update_len - (int)last_update_len
	);
	printf(
		"%zu empty updates, was %zu, delta %i.\n",
		empty_len,
		last_empty_len,
		(int)empty_len - (int)last_empty_len
	);
	printf(
		"%zu nets changed.\n",
		netChangedCount(circ->net.arr, circ->net.len)
	);
	last_update_len = update_len;
	last_empty_len = empty_len;
}

size_t netUpdateLen(struct NetUpdate *head) {
	size_t ret = 0;
	struct NetUpdate *temp = head;
	while (temp) {
		temp = temp->next;
		++ret;
	}
	return ret;
}

size_t netChangedCount(struct Net *arr, size_t len) {
	size_t ret = 0;
	for (size_t i = 0; i < len; ++i) {
		if (arr[i].changed) ++ret;
	}
	return ret;
}

void printCircuit(struct Circuit *circ) {
	int accumulator = busToInt(circ->net.arr, 0, 8);
	int bus = busToInt(circ->net.arr, 10, 8);
	int output = busToInt(circ->net.arr, 69, 8);
	printf(
		"Accumulator: $%02X (%i)\nBus: $%02X (%i)\nOutput: $%02X (%i)\n",
		accumulator, accumulator,
		bus, bus,
		output, output
	);
	printf(
		"~EZ:%i EN:%i EC:%i CF:%i Z:%i N:%i C:%i ES:%i\n",
		circ->net.arr[8].val,
		circ->net.arr[9].val,
		circ->net.arr[34].val,
		circ->net.arr[60].val,
		circ->net.arr[64].val,
		circ->net.arr[76].val,
		circ->net.arr[63].val,
		busToInt(circ->net.arr, 61, 2)
	);
}

int busToInt(struct Net *arr, size_t idx, size_t len) {
	int ret = 0;
	for (size_t i = idx + len - 1; i >= idx && i < idx + len; --i) {
		ret <<= 1;
		ret |= arr[i].val;
	}
	return ret;
}