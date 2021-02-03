#ifndef HDR_DISPLAY
#define HDR_DISPLAY

bool initDisplay(
	uint16_t width,
	uint16_t height,
	uint16_t scale,
	uint64_t interrupt_time
);

void deinitDisplay(
	void
);

void clockDisplay(
	uint64_t step_time
);

#endif