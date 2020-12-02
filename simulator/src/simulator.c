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

int main() {
	initFreeTargetVec(256);
	struct Circuit circ;
	buildCircuit(&circ);
	initCircuit(&circ);
	while (1) {
		char input[32];
		printNetUpdateStatus(&circ);
		printf("> ");
		fgets(input, 32, stdin);
		printf("\n");
		if (strcmp(input, "quit") == 0) exit(0);
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