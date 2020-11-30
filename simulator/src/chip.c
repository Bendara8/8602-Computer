#include "chip.h"
#include "error.h"

void initChip(struct Chip *chip, enum ChipType type, size_t in_len, size_t out_len) {
	chip->type = type;
	chip->state = STATE_STABLE;
	chip->delay = 0;
	chip->in.arr = malloc(sizeof (struct Net *) * in_len);
	if (!chip->in.arr) raiseAbort(
		ABORT_ALLOCATION
	);
	chip->in.len = in_len;
	chip->out.arr = malloc(sizeof (struct Net *) * out_len);
	if (!chip->out.arr) raiseAbort(
		ABORT_ALLOCATION
	);
	chip->out.len = out_len;
	chip->mem.arr = NULL;
	chip->mem.len = 0;
	chip->val = 0;
}

void initChipMemory(struct Chip *chip, size_t len) {
	chip->mem.arr = malloc(sizeof (unsigned char) * len);
	if (!chip->mem.arr) raiseAbort(
		ABORT_ALLOCATION
	);
	chip->mem.len = len;
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