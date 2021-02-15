#ifndef HDR_DISPLAY
#define HDR_DISPLAY

bool initDisplay(
	void
);

void deinitDisplay(
	void
);

void clockDisplay(
	uint64_t step_time
);

void drawDisplay(
	void
);

void setDimensions(
	uint16_t width,
	uint16_t height
);

void setScale(
	uint16_t scale
);

#endif