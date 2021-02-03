#include "keyboard.h"
#include "key-table.h"
#include <allegro5/allegro.h>
#include <stdio.h>

static const uint64_t
	CLOCK_PERIOD = 59880,     // period (ns) of keyboard clock (16.7 kHz)
	PRETRANSMIT_TIME = 50000; // time (ns) to wait after an inhibit

static enum State {
	STATE_TX_INHIBIT,
	STATE_TX_PRETRANSMIT,
	STATE_TX_DATA,
	STATE_TX_CLOCKDOWN,
	STATE_TX_CLOCKUP,
} state = STATE_TX_PRETRANSMIT;

static bool
	host_clock = true, host_data = true,
	dev_clock = true, dev_data = true;

static ALLEGRO_EVENT_QUEUE *queue;

bool getKeyCodes(uint8_t *data);
bool searchKeyTable(int allegro_code, uint8_t *code, bool *extended);
bool calcOddParity(uint8_t data);

bool initKeyboard(void) {
	if(!al_is_system_installed() && !al_init()) {
		puts("Could not initialize Allegro");
		return false;
	}
	if (!al_install_keyboard()) {
		puts("Could not initialize Allegro keyboard");
		return false;
	}
	queue = al_create_event_queue();
	if (!queue) {
		puts("Could not create Allegro event queue");
		return false;
	}
	al_register_event_source(queue, al_get_keyboard_event_source());
	return true;
}

void deinitKeyboard(void) {
	al_uninstall_keyboard();
	al_destroy_event_queue(queue);
}

void stepKeyboard(uint64_t step_time) {
	static uint64_t elapsed_time = 0;
	static uint8_t data[4] = {0};
	static size_t idx = 0;
	static uint8_t byte = 0;
	static uint8_t bit = 0;
	elapsed_time += step_time;
	switch (state) {
		case STATE_TX_INHIBIT:
			if (getKeyClock()) state = STATE_TX_PRETRANSMIT;
			elapsed_time = 0;
			break;

		case STATE_TX_PRETRANSMIT:
			if (elapsed_time >= PRETRANSMIT_TIME) {
				elapsed_time = PRETRANSMIT_TIME;
				if (!al_is_event_queue_empty(queue)) {
					if (!getKeyCodes(data)) {
						state = STATE_TX_PRETRANSMIT;
					}
					else {
						state = STATE_TX_CLOCKUP;
						idx = 0;
						bit = 10;
						elapsed_time = 0;
					}
				}
			}
			break;

		case STATE_TX_DATA:
			if      (bit == 0)  dev_data = false;
			else if (bit <= 8)  dev_data = (byte >> (bit - 1)) & 0x01;
			else if (bit == 9)  dev_data = calcOddParity(byte);
			else if (bit == 10) dev_data = true;
			if (elapsed_time >= CLOCK_PERIOD / 4) {
				state = STATE_TX_CLOCKDOWN;
				elapsed_time -= CLOCK_PERIOD / 4;
			}
			break;

		case STATE_TX_CLOCKDOWN:
			dev_clock = false;
			if (elapsed_time >= CLOCK_PERIOD / 2) {
				state = STATE_TX_CLOCKUP;
				elapsed_time -= CLOCK_PERIOD / 2;
			}
			break;

		case STATE_TX_CLOCKUP:
			dev_clock = true;
			if (elapsed_time >= CLOCK_PERIOD / 4) {
				if (bit == 10) {
					if (data[idx] == 0x00) {
						state = STATE_TX_PRETRANSMIT;
						elapsed_time = PRETRANSMIT_TIME;
					}
					else {
						state = STATE_TX_DATA;
						byte = data[idx];
						++idx;
						bit = 0;
						elapsed_time = 0;
					}
				}
				else {
					state = STATE_TX_DATA;
					++bit;
				}
			}
			break;
	}
}

bool getKeyCodes(uint8_t *data) {
	ALLEGRO_EVENT event;
	al_get_next_event(queue, &event);
	uint8_t code = 0x00;
	bool extended = false;
	switch (event.type) {
		case ALLEGRO_EVENT_KEY_DOWN:
			if (!searchKeyTable(event.keyboard.keycode, &code, &extended)) return false;
			if (extended) {
				data[0] = 0xE0;
				data[1] = code;
				data[2] = 0x00;
			}
			else {
				data[0] = code;
				data[1] = 0x00;
			}
			break;

		case ALLEGRO_EVENT_KEY_UP:
			if (!searchKeyTable(event.keyboard.keycode, &code, &extended)) return false;
			if (extended) {
				data[0] = 0xE0;
				data[1] = 0xF0;
				data[2] = code;
				data[3] = 0x00;
			}
			else {
				data[0] = 0xF0;
				data[1] = code;
				data[2] = 0x00;
			}
			break;

		default:
			data[0] = 0x00;
			break;
	}
	return true;
}

bool searchKeyTable(int allegro_code, uint8_t *code, bool *extended) {
	for (size_t i = 0; i < KEY_TABLE_LEN; ++i) {
		if (KEY_TABLE[i].allegro_code == allegro_code) {
			*code = KEY_TABLE[i].code;
			*extended = KEY_TABLE[i].extended;
			return true;
		}
	}
	return false;
}

bool calcOddParity(uint8_t data) {
	data ^= data >> 4;
	data ^= data >> 2;
	data ^= data >> 1;
	return !(bool)(data & 0x01);
}

bool getKeyClock(void) {
	return host_clock && dev_clock;
}

bool getKeyData(void) {
	return host_data && dev_data;
}

void pullKeyClockLow(bool value) {
	host_clock = value;
	if (!value) state = STATE_TX_INHIBIT;
}

void pullKeyDataLow(bool value) {
	host_data = value;
}