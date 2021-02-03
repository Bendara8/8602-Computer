#ifndef HDR_KEYBOARD
#define HDR_KEYBOARD

bool initKeyboard(
	void
);

void deinitKeyboard(
	void
);

void clockKeyboard(
	uint64_t step_time
);

bool getKeyClock(
	void
);

bool getKeyData(
	void
);

void pullKeyClockLow(
	bool value
);

void pullKeyDataLow(
	bool value
);

#endif