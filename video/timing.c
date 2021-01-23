/* timing.c
 * generates ROM timing data for video output
 */

#include <stdlib.h>
#include <stdio.h>

const size_t FILE_SIZE = 524288;
const size_t X_WIDTH = 9;
const size_t X_SIZE = 512;
const size_t HBLNK_START = 0;
const size_t HBLNK_END = 200;
const size_t HSYN_START = 210;
const size_t HSYN_END = 242;
const size_t X_MAX = 264;
const size_t Y_SIZE = 1024;
const size_t VBLNK_START = 0;
const size_t VBLNK_END = 600;
const size_t VSYN_START = 601;
const size_t VSYN_END = 605;
const size_t Y_MAX = 628;

const char XRST = 0x01;
const char YRST = 0x02;
const char YCLK = 0x04;
const char HSYN = 0x08;
const char VSYN = 0x10;
const char BLNK = 0x20;

int main() {
	// allocate output buffer
	char *out_buf = malloc(FILE_SIZE);
	if (!out_buf) {
		puts("Could not allocate output buffer.");
		return 1;
	}

	// fill output buffer with 500kB of default output
	for (size_t i = 0; i < FILE_SIZE; ++i) {
		out_buf[i] = XRST | YRST | HSYN | VSYN | BLNK;
	}

	// pull XRST low and YCLK high at end of each line
	for (size_t y = 0; y < Y_SIZE; ++y) {
		out_buf[(y << X_WIDTH) | X_MAX] &= ~XRST;
		out_buf[(y << X_WIDTH) | X_MAX] |= YCLK;
	}

	// pull YRST low at end of frame
	out_buf[(Y_MAX << X_WIDTH) | X_MAX] &= ~YRST;

	// pull HSYN low during horizontal sync pulse
	for (size_t y = 0; y < Y_SIZE; ++y) {
		for (size_t x = HSYN_START; x < HSYN_END; ++x) {
			out_buf[(y << X_WIDTH) | x] &= ~HSYN;
		}
	}

	// pull VSYN low during vertical sync pulse
	for (size_t y = VSYN_START; y < VSYN_END; ++y) {
		for (size_t x = 0; x < X_SIZE; ++x) {
			out_buf[(y << X_WIDTH) | x] &= ~VSYN;
		}
	}

	// pull BNLK low during visible area of frame
	for (size_t y = VBLNK_START; y < VBLNK_END; ++y) {
		for (size_t x = HBLNK_START; x < HBLNK_END; ++x) {
			out_buf[(y << X_WIDTH) | x] &= ~BLNK;
		}
	}

	// open output file
	FILE *out_file = fopen("timing.bin", "wb");
	if (!out_file) {
		puts("Could not open 'timing.bin'.");
		return 1;
	}

	// write output buffer to file
	for (size_t i = 0; i < FILE_SIZE; ++i) {
		char c = out_buf[i];
		putc(c, out_file);
	}

	return 0;
}