#include <stdlib.h>
#include <stdio.h> // TEMP
#include "circuit.h"
#include "chip.h"
#include "net.h"
#include "error.h"

void buildCircuit(struct Circuit *circ) {
	circ->chip.len = 18;
	circ->chip.arr = malloc(sizeof (struct Chip) * circ->chip.len);
	if (!circ->chip.arr) raiseAbort(
		ABORT_ALLOCATION
	);
	addFreeTarget(circ->chip.arr);
	circ->net.len = 82;
	circ->net.arr = malloc(sizeof (struct Net) * circ->net.len);
	if (!circ->net.arr) raiseAbort(
		ABORT_ALLOCATION
	);
	addFreeTarget(circ->net.arr);

	// define chips
	initChip(&circ->chip.arr[0],  CHIP_74HC08,  circ);
	initChip(&circ->chip.arr[1],  CHIP_74HC08,  circ);
	initChip(&circ->chip.arr[2],  CHIP_74HC86,  circ);
	initChip(&circ->chip.arr[3],  CHIP_74HC86,  circ);
	initChip(&circ->chip.arr[4],  CHIP_74HC283, circ);
	initChip(&circ->chip.arr[5],  CHIP_74HC283, circ);
	initChip(&circ->chip.arr[6],  CHIP_74HC08,  circ);
	initChip(&circ->chip.arr[7],  CHIP_74HC08,  circ);
	initChip(&circ->chip.arr[8],  CHIP_74HC32,  circ);
	initChip(&circ->chip.arr[9],  CHIP_74HC32,  circ);
	initChip(&circ->chip.arr[10], CHIP_74HC153, circ);
	initChip(&circ->chip.arr[11], CHIP_74HC153, circ);
	initChip(&circ->chip.arr[12], CHIP_74HC153, circ);
	initChip(&circ->chip.arr[13], CHIP_74HC153, circ);
	initChip(&circ->chip.arr[14], CHIP_74HC153, circ);
	initChip(&circ->chip.arr[15], CHIP_74HC02,  circ);
	initChip(&circ->chip.arr[16], CHIP_74HC21,  circ);
	initChip(&circ->chip.arr[17], CHIP_74HC377, circ);
	
	// connect nets
	for (size_t i = 0; i < 4; ++i) {
		// input A
		circ->chip.arr[0].in[i * 2] =     &circ->net.arr[i];
		circ->chip.arr[0].in[i * 2 + 1] = &circ->net.arr[8];
		circ->chip.arr[0].out[i] =        &circ->net.arr[i + 18];
		circ->chip.arr[1].in[i * 2] =     &circ->net.arr[i + 4];
		circ->chip.arr[1].in[i * 2 + 1] = &circ->net.arr[8];
		circ->chip.arr[1].out[i] =        &circ->net.arr[i + 22];
		
		// input B
		circ->chip.arr[2].in[i * 2] =     &circ->net.arr[i + 10];
		circ->chip.arr[2].in[i * 2 + 1] = &circ->net.arr[9];
		circ->chip.arr[2].out[i] =        &circ->net.arr[i + 26];
		circ->chip.arr[3].in[i * 2] =     &circ->net.arr[i + 14];
		circ->chip.arr[3].in[i * 2 + 1] = &circ->net.arr[9];
		circ->chip.arr[3].out[i] =        &circ->net.arr[i + 30];

		// sum
		circ->chip.arr[4].in[i] =     &circ->net.arr[i + 18];
		circ->chip.arr[4].in[i + 4] = &circ->net.arr[i + 26];
		circ->chip.arr[4].out[i] =    &circ->net.arr[i + 36];
		circ->chip.arr[5].in[i] =     &circ->net.arr[i + 22];
		circ->chip.arr[5].in[i + 4] = &circ->net.arr[i + 30];
		circ->chip.arr[5].out[i] =    &circ->net.arr[i + 40];
		
		// and
		circ->chip.arr[6].in[i * 2] =     &circ->net.arr[i + 18];
		circ->chip.arr[6].in[i * 2 + 1] = &circ->net.arr[i + 26];
		circ->chip.arr[6].out[i] =        &circ->net.arr[i + 44];
		circ->chip.arr[7].in[i * 2] =     &circ->net.arr[i + 22];
		circ->chip.arr[7].in[i * 2 + 1] = &circ->net.arr[i + 30];
		circ->chip.arr[7].out[i] =        &circ->net.arr[i + 48];

		// or
		circ->chip.arr[8].in[i * 2] =     &circ->net.arr[i + 18];
		circ->chip.arr[8].in[i * 2 + 1] = &circ->net.arr[i + 26];
		circ->chip.arr[8].out[i] =        &circ->net.arr[i + 52];
		circ->chip.arr[9].in[i * 2] =     &circ->net.arr[i + 22];
		circ->chip.arr[9].in[i * 2 + 1] = &circ->net.arr[i + 30];
		circ->chip.arr[9].out[i] =        &circ->net.arr[i + 56];

		// zero test
		circ->chip.arr[15].out[i] =    &circ->net.arr[i + 65];
		circ->chip.arr[16].in[i] =     &circ->net.arr[i + 65];
		circ->chip.arr[16].in[i + 4] = &circ->net.arr[78];
	}
	for (size_t i = 0; i < 2; ++i) {
		// output selector
		circ->chip.arr[10].in[i] =      &circ->net.arr[i + 36];
		circ->chip.arr[10].in[i + 2] =  &circ->net.arr[i + 27];
		circ->chip.arr[10].in[i + 4] =  &circ->net.arr[i + 44];
		circ->chip.arr[10].in[i + 6] =  &circ->net.arr[i + 52];
		circ->chip.arr[10].in[i + 8] =  &circ->net.arr[i + 61];
		circ->chip.arr[10].in[i + 10] = &circ->net.arr[78];
		circ->chip.arr[10].out[i] =     &circ->net.arr[i + 69];
		circ->chip.arr[11].in[i] =      &circ->net.arr[i + 38];
		circ->chip.arr[11].in[i + 2] =  &circ->net.arr[i + 29];
		circ->chip.arr[11].in[i + 4] =  &circ->net.arr[i + 46];
		circ->chip.arr[11].in[i + 6] =  &circ->net.arr[i + 54];
		circ->chip.arr[11].in[i + 8] =  &circ->net.arr[i + 61];
		circ->chip.arr[11].in[i + 10] = &circ->net.arr[78];
		circ->chip.arr[11].out[i] =     &circ->net.arr[i + 71];
		circ->chip.arr[12].in[i] =      &circ->net.arr[i + 40];
		circ->chip.arr[12].in[i + 2] =  &circ->net.arr[i + 31];
		circ->chip.arr[12].in[i + 4] =  &circ->net.arr[i + 48];
		circ->chip.arr[12].in[i + 6] =  &circ->net.arr[i + 56];
		circ->chip.arr[12].in[i + 8] =  &circ->net.arr[i + 61];
		circ->chip.arr[12].in[i + 10] = &circ->net.arr[78];
		circ->chip.arr[12].out[i] =     &circ->net.arr[i + 73];
		circ->chip.arr[13].in[i] =      &circ->net.arr[i + 42];
		circ->chip.arr[13].in[i + 4] =  &circ->net.arr[i + 50];
		circ->chip.arr[13].in[i + 6] =  &circ->net.arr[i + 58];
		circ->chip.arr[13].in[i + 8] =  &circ->net.arr[i + 61];
		circ->chip.arr[13].in[i + 10] = &circ->net.arr[78];
		circ->chip.arr[13].out[i] =     &circ->net.arr[i + 75];

		// carry selector
		circ->chip.arr[14].in[i + 8] =  &circ->net.arr[i + 61];
		circ->chip.arr[14].in[i + 10] = &circ->net.arr[78];
	}
	for (size_t i = 0; i < 8; ++i) {
		// zero test
		circ->chip.arr[15].in[i] = &circ->net.arr[i + 69];

		// accumulator
		circ->chip.arr[17].in[i] =  &circ->net.arr[i + 69];
		circ->chip.arr[17].out[i] = &circ->net.arr[i];
	}
	// sum
	circ->chip.arr[4].in[8] =  &circ->net.arr[34];
	circ->chip.arr[4].out[4] = &circ->net.arr[77];
	circ->chip.arr[5].in[8] =  &circ->net.arr[77];
	circ->chip.arr[5].out[4] = &circ->net.arr[35];

	// output selector
	circ->chip.arr[13].in[2] = &circ->net.arr[33];
	circ->chip.arr[13].in[3] = &circ->net.arr[34];

	// carry selector
	circ->chip.arr[14].in[0] =  &circ->net.arr[35];
	circ->chip.arr[14].in[1] =  &circ->net.arr[78];
	circ->chip.arr[14].in[2] =  &circ->net.arr[26];
	circ->chip.arr[14].in[3] =  &circ->net.arr[78];
	circ->chip.arr[14].in[4] =  &circ->net.arr[60];
	circ->chip.arr[14].in[5] =  &circ->net.arr[78];
	circ->chip.arr[14].in[6] =  &circ->net.arr[60];
	circ->chip.arr[14].in[7] =  &circ->net.arr[78];
	circ->chip.arr[14].out[0] = &circ->net.arr[63];
	circ->chip.arr[14].out[1] = NULL;

	// zero test
	circ->chip.arr[16].out[0] = &circ->net.arr[64];
	circ->chip.arr[16].out[1] = NULL;

	// accumulator
	circ->chip.arr[17].in[8] = &circ->net.arr[80];
	circ->chip.arr[17].in[9] = &circ->net.arr[81];
}

void initCircuit(struct Circuit *circ) {
	for (size_t i = 0; i < circ->net.len; ++i) {
		circ->net.arr[i].val = 0;
		circ->net.arr[i].changed = 1;
	}
	circ->net.arr[8].val = 1;
	circ->net.arr[80].val = 1;
	circ->net.arr[79].val = 1;

	// init bus input
	//circ->net.arr[10].val = 1;
	//circ->net.arr[12].val = 1;
	//circ->net.arr[14].val = 1;

	circ->update.head = NULL;
	circ->empty.head = allocNetUpdateBlock(512);
}

void stepCircuit(struct Circuit *circ) {
	// step chips and add net updates
	for (size_t i = 0; i < circ->chip.len; ++i) {
		stepChip(&circ->chip.arr[i]);
	}

	// clear changed status
	for (size_t i = 0; i < circ->net.len; ++i) {
		circ->net.arr[i].changed = 0;
	}

	// step and apply net updates
	struct NetUpdate *temp = circ->update.head, **last = &circ->update.head;
	while (temp) {
		last = stepNetUpdate(temp, last, &circ->empty.head);
		temp = *last;
	}
}

void addNetUpdate(struct Circuit *circ, struct Net *target, int val, int delay) {
	if (val != 0 && val != 1) {
		printf("Adding out of bounds net...\n");
	}
	if (!target) return;
	struct NetUpdate *temp = circ->update.head, **last = &circ->update.head;
	while (temp) {
		if (temp->target == target && temp->delay >= delay) {
			*last = temp->next;
			temp->next = circ->empty.head;
			circ->empty.head = temp;
			temp->target = NULL;
			temp->val = 0;
			temp->delay = 0;
		}
		else last = &temp->next;
		temp = *last;
	}
	if (!circ->empty.head) circ->empty.head = allocNetUpdateBlock(512);
	temp = circ->empty.head->next;
	circ->empty.head->next = circ->update.head;
	circ->update.head = circ->empty.head;
	circ->empty.head = temp;
	circ->update.head->target = target;
	circ->update.head->val = val;
	circ->update.head->delay = delay;
}