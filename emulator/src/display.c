#include "display.h"
#include "interrupt.h"
#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

static const uint16_t MAX_WIDTH = 512, MAX_HEIGHT = 256;

static uint16_t width, height, scale; // display dimensions
static uint64_t interrupt_time;       // time (ns) between interrupts
static uint8_t *vram_buf;             // 32kB VRAM
static ALLEGRO_DISPLAY *disp = NULL;

static void drawSubPixel(uint16_t x, uint16_t y,	ALLEGRO_COLOR color);
static ALLEGRO_COLOR toPixelColor(uint8_t pixel);
static ALLEGRO_COLOR toTextColor(uint8_t pixel);

bool initDisplay(
	uint16_t w, uint16_t h, uint16_t s,
	uint64_t i,
	uint8_t *v
) {
	// init globals
	width = w;
	height = h;
	scale = s;
	interrupt_time = i;
	vram_buf = v;

	// check globals
	if (width > MAX_WIDTH) {
		printf("Display width (%hu) too large (max %hu)\n", width, MAX_WIDTH);
		return false;
	}
	if (height > MAX_HEIGHT) {
		printf("Display height (%hu) too large (max %hu)\n", height, MAX_HEIGHT);
		return false;
	}

	// init allegro display
	if(!al_init()) {
		puts("Could not initialize Allegro");
		return false;
	}
	if (!al_init_primitives_addon()) {
		puts("Could not initialize Allegro primitives");
		return false;
	}
	disp = al_create_display(width * scale, height * scale);
	if (!disp) {
		puts("Could not create Allegro display");
		return false;
	}
	al_set_window_title(disp, "8602 Emulator");
	return true;
}

void deinitDisplay(void) {
	al_shutdown_primitives_addon();
	al_destroy_display(disp);
}

void stepDisplay(uint64_t step_time) {
	static uint64_t elapsed = 0;
	if (elapsed >= interrupt_time) {
		generateInterrupt(INTER_0);
		elapsed = 0;
	}
	else {
		elapsed += step_time;
	}
}

void updateDisplay(void) {
	static uint8_t back_pixel = 0;
	for (uint16_t y = 0; y < height / 2; ++y) {
		for (uint16_t x = 0; x < width / 2; ++x) {
			uint8_t pixel = vram_buf[(y << 8) | x];
			uint16_t base_x = x * 2 * scale;
			uint16_t base_y = y * 2 * scale;
			if (pixel & 0x80) {
				// subpixel text mode
				ALLEGRO_COLOR color = toTextColor(pixel);
				ALLEGRO_COLOR back_color = toPixelColor(back_pixel);
				drawSubPixel(base_x + 0, base_y + 0, (pixel & 0x08) ? color : back_color);
				drawSubPixel(base_x + 1, base_y + 0, (pixel & 0x04) ? color : back_color);
				drawSubPixel(base_x + 0, base_y + 1, (pixel & 0x02) ? color : back_color);
				drawSubPixel(base_x + 1, base_y + 1, (pixel & 0x01) ? color : back_color);
			}
			else {
				// full color pixel mode
				if (pixel & 0x40) {
					back_pixel = pixel & 0x3F;
				}
				al_draw_filled_rectangle(
					base_x, base_y,
					base_x + (scale * 2), base_y + (scale * 2),
					toPixelColor(pixel)
				);
			}
		}
	}
}

void drawSubPixel(uint16_t x, uint16_t y,	ALLEGRO_COLOR color) {
	al_draw_filled_rectangle(
		x, y, x + scale, y + scale,
		color
	);
}

ALLEGRO_COLOR toPixelColor(uint8_t pixel) {
	static const uint8_t value_table[4] = {
		0x00, 0x55, 0xAA, 0xFF
	};
	return al_map_rgb(
		value_table[(pixel & 0x30) >> 4], // red
		value_table[(pixel & 0x0C) >> 2], // green
		value_table[(pixel & 0x03) >> 0]  // blue
	);
}

ALLEGRO_COLOR toTextColor(uint8_t pixel) {
	static const uint8_t color_table[8] = {
		0x2A, // grey
		0x06, // blue
		0x08, // green
		0x0A, // teal
		0x20, // red
		0x38, // orange
		0x3C, // yellow
		0x3F, // white
	};
	return toPixelColor(color_table[(pixel & 0x70) >> 4]);
}