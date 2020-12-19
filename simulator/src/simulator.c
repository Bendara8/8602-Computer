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
 * Interlaced video? could get full 256x128 resolution using VESA 1024x768
 */

#include "circuit.h"
#include "chip.h"
#include "net.h"
#include "interface.h"
#include "keyboard.h"
#include "display.h"
#include "error.h"

int main(int arg_arr_len, char **arg_arr) {
	printInfo();
	if (arg_arr_len != 2) raiseAbort(ABORT_NUM_ARGS);
	initFreeTargetVec(256);
	struct Circuit circ;
	initCircuit(&circ, arg_arr[1]);
	enterInterface(&circ);
}

