#include <stdlib.h>
#include "chip.h"
#include "error.h"

void initChip(struct Chip *chip, enum ChipType type) {
	chip->type = type;
	if (CHIP_DATA[type].in != 0) {
		chip->in = malloc(sizeof (struct Net *) * CHIP_DATA[type].in);
		if (!chip->in) raiseAbort(
			ABORT_ALLOCATION
		);
	}
	else {
		chip->in = NULL;
	}
	if (CHIP_DATA[type].out != 0) {
		chip->out = malloc(sizeof (struct Net *) * CHIP_DATA[type].out);
		if (!chip->out) raiseAbort(
			ABORT_ALLOCATION
		);
	}
	else {
		chip->out = NULL;
	}
	if (CHIP_DATA[type].local != 0) {
		chip->local = malloc(sizeof (struct Net) * CHIP_DATA[type].local);
		if (!chip->local) raiseAbort(
			ABORT_ALLOCATION
		);
	}
	else {
		chip->local = NULL;
	}
	if (CHIP_DATA[type].mem != 0) {
		chip->mem = malloc(sizeof (unsigned char) * CHIP_DATA[type].mem);
		if (!chip->mem) raiseAbort(
			ABORT_ALLOCATION
		);
	}
	else {
		chip->mem = NULL;
	}
}

void stepChip(struct Chip *chip) {
	switch (chip->type) {
		case CHIP_74HC00:
			step74HC00(chip);
			break;
		case CHIP_74HC377:
			step74HC377(chip);
			break;
	}
}

void step74HC00(struct Chip *chip) {
	int val;
	for (size_t i = 0; i < 4; ++i) {
		if (chip->in[i * 2]->changed | chip->in[i * 2 + 1]->changed) {
			val = ~(chip->in[i * 2]->val & chip->in[i * 2 + 1]->val);
			addNetUpdate(chip->circ, chip->out[i], val, 7);
		}
	}
}

void step74HC377(struct Chip *chip) {
	if (chip->in[0]->changed) {
		addNetUpdate(chip->circ, &chip->local[0], chip->in[0]->val, 5);
	}
	if (chip->in[1]->changed) {
		addNetUpdate(chip->circ, &chip->local[1], chip->in[1]->val, 2);
	}
	if (chip->in[3]->changed && chip->in[3]->val && !chip->local[1].val) {
		addNetUpdate(chip->circ, chip->out[0], chip->local[0].val, 13);
	}
}