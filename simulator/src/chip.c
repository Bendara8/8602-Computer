#include <stdlib.h>
#include "chip.h"
#include "circuit.h"
#include "net.h"
#include "error.h"

void initChip(struct Chip *chip, enum ChipType type) {
	chip->type = type;
	if (CHIP_DATA[type].in != 0) {
		chip->in = malloc(sizeof (struct Net *) * CHIP_DATA[type].in);
		if (!chip->in) raiseAbort(
			ABORT_ALLOCATION
		);
		addFreeTarget(chip->in);
	}
	else {
		chip->in = NULL;
	}
	if (CHIP_DATA[type].out != 0) {
		chip->out = malloc(sizeof (struct Net *) * CHIP_DATA[type].out);
		if (!chip->out) raiseAbort(
			ABORT_ALLOCATION
		);
		addFreeTarget(chip->out);
	}
	else {
		chip->out = NULL;
	}
	if (CHIP_DATA[type].local != 0) {
		chip->local = malloc(sizeof (struct Net) * CHIP_DATA[type].local);
		if (!chip->local) raiseAbort(
			ABORT_ALLOCATION
		);
		addFreeTarget(chip->local);
	}
	else {
		chip->local = NULL;
	}
	if (CHIP_DATA[type].mem != 0) {
		chip->mem = malloc(sizeof (unsigned char) * CHIP_DATA[type].mem);
		if (!chip->mem) raiseAbort(
			ABORT_ALLOCATION
		);
		addFreeTarget(chip->mem);
	}
	else {
		chip->mem = NULL;
	}
}

void stepChip(struct Chip *chip) {
	switch (chip->type) {
		case CHIP_74HC00:  step74HC00(chip); break;
		case CHIP_74HC02:  step74HC02(chip); break;
		case CHIP_74HC08:  step74HC08(chip); break;
		case CHIP_74HC32:  step74HC32(chip); break;
		case CHIP_74HC86:  step74HC86(chip); break;
		case CHIP_74HC21:  step74HC21(chip); break;
		case CHIP_74HC157: step74HC157(chip); break;
		case CHIP_74HC153: step74HC153(chip); break;
		case CHIP_74HC283: step74HC283(chip); break;
		case CHIP_74HC377: step74HC377(chip); break;
	}
}

void step74HC00(struct Chip *chip) {
	int val;
	for (size_t i = 0; i < 4; ++i) {
		if (chip->in[i * 2]->changed || chip->in[i * 2 + 1]->changed) {
			val = ~(chip->in[i * 2]->val & chip->in[i * 2 + 1]->val);
			addNetUpdate(chip->circ, chip->out[i], val, 7);
		}
	}
}

void step74HC02(struct Chip *chip) {
	int val;
	for (size_t i = 0; i < 4; ++i) {
		if (chip->in[i * 2]->changed || chip->in[i * 2 + 1]->changed) {
			val = ~(chip->in[i * 2]->val | chip->in[i * 2 + 1]->val);
			addNetUpdate(chip->circ, chip->out[i], val, 7);
		}
	}
}

void step74HC08(struct Chip *chip) {
	int val;
	for (size_t i = 0; i < 4; ++i) {
		if (chip->in[i * 2]->changed || chip->in[i * 2 + 1]->changed) {
			val = chip->in[i * 2]->val & chip->in[i * 2 + 1]->val;
			addNetUpdate(chip->circ, chip->out[i], val, 7);
		}
	}
}

void step74HC32(struct Chip *chip) {
	int val;
	for (size_t i = 0; i < 4; ++i) {
		if (chip->in[i * 2]->changed || chip->in[i * 2 + 1]->changed) {
			val = chip->in[i * 2]->val | chip->in[i * 2 + 1]->val;
			addNetUpdate(chip->circ, chip->out[i], val, 7);
		}
	}
}

void step74HC86(struct Chip *chip) {
	int val;
	for (size_t i = 0; i < 4; ++i) {
		if (chip->in[i * 2]->changed || chip->in[i * 2 + 1]->changed) {
			val = chip->in[i * 2]->val ^ chip->in[i * 2 + 1]->val;
			addNetUpdate(chip->circ, chip->out[i], val, 11);
		}
	}
}

void step74HC21(struct Chip *chip) {
	int val;
	for (size_t i = 0; i < 2; ++i) {
		if (
			chip->in[i * 4]->changed ||
			chip->in[i * 4 + 1]->changed ||
			chip->in[i * 4 + 2]->changed ||
			chip->in[i * 4 + 3]->changed
		) {
			val = 
				chip->in[i * 4]->val &
				chip->in[i * 4 + 1]->val &
				chip->in[i * 4 + 2]->val &
				chip->in[i * 4 + 3]->val;
			addNetUpdate(chip->circ, chip->out[i], val, 12);
		}
	}
}

void step74HC157(struct Chip *chip) {
	if (chip->in[9]->changed && chip->in[9]->val) {
		for (size_t i = 0; i < 4; ++i) {
			addNetUpdate(chip->circ, chip->out[i], 0, 14);
		}
	}
	else if (!chip->in[9]->val) {
		size_t offset = chip->in[8]->val * 4;
		if (chip->in[8]->changed) {
			for (size_t i = 0; i < 4; ++i) {
				addNetUpdate(chip->circ, chip->out[i], chip->in[i + offset]->val, 15);
			}
		}
		else for (size_t i = 0; i < 4; ++i) {
			if (chip->in[i + offset]->changed) {
				addNetUpdate(chip->circ, chip->out[i], chip->in[i + offset]->val, 13);
			}
		}
	}
}

void step74HC153(struct Chip *chip) {
	if (
		(chip->in[10]->changed && chip->in[10]->val) ||
		(chip->in[11]->changed && chip->in[11]->val)
	) {
		for (size_t i = 0; i < 2; ++i) {
			addNetUpdate(chip->circ, chip->out[i], 0, 12);
		}
	}
	else if (!chip->in[10]->val && !chip->in[11]->val) {
		size_t offset = (chip->in[10]->val * 2) + (chip->in[11]->val * 4);
		if (chip->in[10]->changed || chip->in[11]->changed) {
			for (size_t i = 0; i < 2; ++i) {
				addNetUpdate(chip->circ, chip->out[i], chip->in[i + offset]->val, 18);
			}
		}
		else for (size_t i = 0; i < 2; ++i) {
			if (chip->in[i + offset]->changed) {
				addNetUpdate(chip->circ, chip->out[i], chip->in[i + offset]->val, 17);
			}
		}
	}
}

static void step74HC283Add(struct Chip *chip, int *delay) {
	int a = 0, b = 0;
	for (size_t j = 0; j < 4; ++j) {
		a <<= 1;
		a |= chip->in[j]->val;
		b <<= 1;
		b |= chip->in[j + 4]->val;
	}
	int val = a + b + chip->in[8]->val;
	for (size_t j = 0; j < 5; ++j) {
		addNetUpdate(chip->circ, chip->out[j], val & 1, delay[j]);
		val >>= 1;
	}
}

void step74HC283(struct Chip *chip) {
	if (chip->in[8]->changed) {
		int delay[] = {19, 21, 23, 27, 23};
		step74HC283Add(chip, delay);
	}
	else for (size_t i = 0; i < 8; ++i) {
		if (chip->in[i]->changed) {
			int delay[] = {25, 25, 25, 25, 23};
			step74HC283Add(chip, delay);
			break;
		}
	}
}

void step74HC377(struct Chip *chip) {
	for (size_t i = 0; i < 8; ++i) {
		if (chip->in[i]->changed) {
			addNetUpdate(chip->circ, &chip->local[i], chip->in[i]->val, 5);
		}
	}
	if (chip->in[8]->changed) {
		addNetUpdate(chip->circ, &chip->local[8], chip->in[8]->val, 2);
	}
	if (chip->in[9]->changed && chip->in[9]->val && !chip->local[8].val) {
		for (size_t i = 0; i < 8; ++i) {
			addNetUpdate(chip->circ, chip->out[i], chip->local[i].val, 13);
		}
	}
}