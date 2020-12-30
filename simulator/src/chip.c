#include <stdlib.h>
#include <string.h>
#include "chip.h"
#include "circuit.h"
#include "net.h"
#include "error.h"

void initChip(struct Chip *chip, enum ChipType type, char *name, struct Circuit *circ) {
	chip->type = type;
	chip->name = name;
	chip->circ = circ;
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
		for (size_t i = 0; i < CHIP_DATA[type].local; ++i) {
			chip->local[i].name = NULL;
			chip->local[i].val = 0;
		}
	}
	else {
		chip->local = NULL;
	}
	if (CHIP_DATA[type].mem != 0) {
		chip->mem = malloc(sizeof (unsigned char) * (1 << CHIP_DATA[type].mem));
		if (!chip->mem) raiseAbort(
			ABORT_ALLOCATION
		);
		addFreeTarget(chip->mem);
	}
	else {
		chip->mem = NULL;
	}
}

void initChipVec(struct ChipVec *vec, size_t cap) {
	vec->buf = malloc(sizeof (struct Chip) * cap);
	if (!vec->buf) raiseAbort(
		ABORT_ALLOCATION
	);
	addFreeTarget(vec->buf);
	vec->cap = cap;
	vec->len = 0;
}

struct Chip *addChip(struct ChipVec *vec, char *type_str, char *name, struct Circuit *circ) {
	if (vec->len == vec->cap) {
		vec->cap *= 2;
		void **target = findFreeTarget(vec->buf);
		if (!target) raiseAbort(ABORT_NO_FREE_TARGET);
		vec->buf = realloc(vec->buf, sizeof (struct Chip) * vec->cap);
		if (!vec->buf) raiseAbort(
			ABORT_ALLOCATION
		);
		*target = vec->buf;
	}
	initChip(&vec->buf[vec->len], strToChipType(type_str), name, circ);
	return &vec->buf[vec->len++];
}

struct Chip *findChip(struct ChipVec *vec, char *name) {
	for (size_t i = 0; i < vec->len; ++i) {
		if (vec->buf[i].name && strcmp(vec->buf[i].name, name) == 0) return &vec->buf[i];
	}
	return NULL;
}

void stepChip(struct Chip *chip) {
	switch (chip->type) {
		case CHIP_CLK40M:  stepCLK40M(chip); break;
		case CHIP_74HC00:  step74HC00(chip); break;
		case CHIP_74HC30:  step74HC30(chip); break;
		case CHIP_74HC02:  step74HC02(chip); break;
		case CHIP_74HC04:  step74HC04(chip); break;
		case CHIP_74HC08:  step74HC08(chip); break;
		case CHIP_74HC21:  step74HC21(chip); break;
		case CHIP_74HC32:  step74HC32(chip); break;
		case CHIP_74HC86:  step74HC86(chip); break;
		case CHIP_74HC157: step74HC157(chip); break;
		case CHIP_74HC153: step74HC153(chip); break;
		case CHIP_74AC161: step74AC161(chip); break;
		case CHIP_74HC283: step74HC283(chip); break;
		case CHIP_74HC377: step74HC377(chip); break;
	}
}

void stepCLK40M(struct Chip *chip) {
	if (chip->in[0]->val && (chip->in[0]->changed || chip->out[0]->changed)) {
		int delay = (chip->out[0]->val) ? 12 : 13;
		addNetUpdate(chip->circ, chip->out[0], !chip->out[0]->val, delay);
	}
}

void step74HC00(struct Chip *chip) {
	int val;
	for (size_t i = 0; i < 4; ++i) {
		if (chip->in[i * 2]->changed || chip->in[i * 2 + 1]->changed) {
			val = !(chip->in[i * 2]->val && chip->in[i * 2 + 1]->val);
			addNetUpdate(chip->circ, chip->out[i], val, 7);
		}
	}
}

void step74HC30(struct Chip *chip) {
	int val = 1;
	for (size_t i = 0; i < 8; ++i) {
		if (chip->in[i]->changed) {
			for (size_t j = i; j < 8; ++j) {
				val &= chip->in[j]->val;
			}
			addNetUpdate(chip->circ, chip->out[0], !val, 10);
			return;
		}
		val &= chip->in[i]->val;
	}
}

void step74HC02(struct Chip *chip) {
	int val;
	for (size_t i = 0; i < 4; ++i) {
		if (chip->in[i * 2]->changed || chip->in[i * 2 + 1]->changed) {
			val = !(chip->in[i * 2]->val || chip->in[i * 2 + 1]->val);
			addNetUpdate(chip->circ, chip->out[i], val, 7);
		}
	}
}

void step74HC04(struct Chip *chip) {
	for (size_t i = 0; i < 6; ++i) {
		if (chip->in[i]->changed) {
			addNetUpdate(chip->circ, chip->out[i], !chip->in[i]->val, 9);
		}
	}
}

void step74HC08(struct Chip *chip) {
	int val;
	for (size_t i = 0; i < 4; ++i) {
		if (chip->in[i * 2]->changed || chip->in[i * 2 + 1]->changed) {
			val = chip->in[i * 2]->val && chip->in[i * 2 + 1]->val;
			addNetUpdate(chip->circ, chip->out[i], val, 7);
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
				chip->in[i * 4]->val &&
				chip->in[i * 4 + 1]->val &&
				chip->in[i * 4 + 2]->val &&
				chip->in[i * 4 + 3]->val;
			addNetUpdate(chip->circ, chip->out[i], val, 12);
		}
	}
}

void step74HC32(struct Chip *chip) {
	int val;
	for (size_t i = 0; i < 4; ++i) {
		if (chip->in[i * 2]->changed || chip->in[i * 2 + 1]->changed) {
			val = chip->in[i * 2]->val || chip->in[i * 2 + 1]->val;
			addNetUpdate(chip->circ, chip->out[i], val, 7);
		}
	}
}

void step74HC86(struct Chip *chip) {
	int val;
	for (size_t i = 0; i < 4; ++i) {
		if (chip->in[i * 2]->changed || chip->in[i * 2 + 1]->changed) {
			val = chip->in[i * 2]->val != chip->in[i * 2 + 1]->val;
			addNetUpdate(chip->circ, chip->out[i], val, 11);
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
		size_t offset = (chip->in[8]->val * 2) + (chip->in[9]->val * 4);
		if (chip->in[8]->changed || chip->in[9]->changed) {
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

static void step74AC161Count(struct Chip *chip) {
	int ripple = 1;
	for (size_t i = 0; i < 4; ++i) {
		if (!ripple) break;
		addNetUpdate(chip->circ, chip->out[i], !chip->out[i]->val, 15);
		ripple = chip->out[i]->val;
	}
	if (
		chip->out[1]->val &&
		chip->out[2]->val &&
		chip->out[3]->val
	) {
		addNetUpdate(chip->circ, chip->out[4], !chip->out[0]->val, 15);
	}
}

void step74AC161(struct Chip *chip) {
	for (size_t i = 0; i < 5; ++i) {
		if (chip->in[i]->changed) {
			addNetUpdate(chip->circ, &chip->local[i], chip->in[i]->val, 5);
		}
	}
	if (!chip->in[7]->val) {
		if (chip->in[7]->changed) {
			for (size_t i = 0; i < 4; ++i) {
				addNetUpdate(chip->circ, chip->out[i], 0, 15);
			}
			addNetUpdate(chip->circ, chip->out[4], 0, 15);
		}
	}
	else {
		if (chip->in[8]->changed && chip->in[8]->val) {
			if (!chip->in[4]->val) {
				for (size_t i = 0; i < 4; ++i) {
					addNetUpdate(chip->circ, chip->out[i], chip->local[i].val, 15);
				}
			}
			else if (chip->in[5]->val && chip->in[6]->val) {
				step74AC161Count(chip);
			}
		}
		if (chip->in[6]->changed) {
			if (
				chip->in[6]->val &&
				chip->out[0]->val &&
				chip->out[1]->val &&
				chip->out[2]->val &&
				chip->out[3]->val
			) {
				addNetUpdate(chip->circ, chip->out[4], 1, 9);
			}
			else addNetUpdate(chip->circ, chip->out[4], 0, 9);
		}
	}
}

static void step74HC283Add(struct Chip *chip, int *delay) {
	int a = 0, b = 0;
	for (size_t j = 3; j < 4; --j) {
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