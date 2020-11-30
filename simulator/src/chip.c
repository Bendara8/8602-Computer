#include "chip.h"
#include "error.h"

void initChip(struct Chip *chip, enum ChipType type) {
	chip->type = type;
	chip->state = STATE_STABLE;
	chip->delay = 0;
	chip->in.len = CHIP_DATA_TABLE[type].in_len;
	chip->in.arr = malloc(sizeof (struct Net *) * chip->in.len);
	if (!chip->in.arr) raiseAbort(
		ABORT_ALLOCATION
	);
	chip->out.len = CHIP_DATA_TABLE[type].out_len;
	chip->out.arr = malloc(sizeof (struct Net *) * chip->out.len);
	if (!chip->out.arr) raiseAbort(
		ABORT_ALLOCATION
	);
	chip->mem.len = CHIP_DATA_TABLE[type].mem_len;
	if (chip->mem.len != 0) {
		chip->mem.arr = malloc(sizeof (unsigned char) * chip->mem.len);
		if (!chip->mem.arr) raiseAbort(
			ABORT_ALLOCATION
		);
	}
	else {
		chip->mem.arr = NULL;
	}
	chip->val = 0;
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
	switch (chip->state) {
		case STATE_STABLE:
			for (size_t i = 0; i < chip->in.len - 1; i += 2) {
				if (
					chip->in.arr[i]->changed |
					chip->in.arr[i + 1]->changed
				) {
					// set next out and change state... issues with stacking outputs
					// instead schedhule a future net update in a central system? <- do this, linked list of updates
				}
			}
			break;
		case STATE_PROP_DELAY:

			break;
	}
}

void step74HC377(struct Chip *chip) {

}