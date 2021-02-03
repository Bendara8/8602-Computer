#include "cpu.h"
#include "interrupt.h"
#include "alu.h"
#include "control.h"
#include "memory.h"

static uint8_t
	step,
	reset,
	accumulator,
	index,
	bank,
	opcode;

static enum Flag flags;

static uint16_t
	pointer,
	base_pointer,
	stack_pointer,
	instruction_pointer,
	transfer_pointer;


void resetCPU(void) {
	step = 0;
	reset = 1;
}

void clockCPU(void) {
	// get control word
	enum Control control = readControl(
		step, flags, opcode,
		getInterrupt(INTER_0), getInterrupt(INTER_1),
		reset
	);

	// output to address bus
	uint16_t address_bus = 0;
	if (control & CTRL_TO) address_bus = transfer_pointer;
	else switch (control & CTRL_IO) {
		case CTRL_PO: address_bus = pointer; break;
		case CTRL_BO: address_bus = base_pointer; break;
		case CTRL_SO: address_bus = stack_pointer; break;
		case CTRL_IO: address_bus = instruction_pointer; break;
	}

	// index address
	uint16_t index_out = index;
	if (!(control & CTRL_XZ)) index_out = 0;
	if (control & CTRL_XN) index_out = ~index_out;
	uint16_t address_out = address_bus + index_out + (control & CTRL_XC ? 1 : 0);

	// output to data bus
	uint8_t data_bus = 0;
	switch (control & CTRL_OB) {
		case CTRL_AO: data_bus = accumulator; break;
		case CTRL_FO: data_bus = flags; break;
		case CTRL_KO: data_bus = bank; break;
		case CTRL_MO: data_bus = readMemory(address_out); break;
		case CTRL_PL: data_bus = (uint8_t)pointer; break;
		case CTRL_PH: data_bus = (uint8_t)(pointer >> 8); break;
		case CTRL_BL: data_bus = (uint8_t)base_pointer; break;
		case CTRL_BH: data_bus = (uint8_t)(base_pointer >> 8); break;
		case CTRL_IL: data_bus = (uint8_t)instruction_pointer; break;
		case CTRL_IH: data_bus = (uint8_t)(instruction_pointer >> 8); break;
		case CTRL_HO: data_bus = 0xFF; break;
	}

	// calculate alu
	uint8_t alu_out = calcALU(accumulator, data_bus, &flags);

	// load from address output
	if (control & CTRL_NA) switch (control & CTRL_II) {
		case CTRL_PI: pointer = address_out; break;
		case CTRL_BI: base_pointer = address_out; break;
		case CTRL_SI: stack_pointer = address_out; break;
		case CTRL_II: instruction_pointer = address_out; break;
	}

	// load from data bus and alu
	switch (control & CTRL_ND) {
		case CTRL_AI: accumulator = alu_out; break;
		case CTRL_XI: index = data_bus; break;
		case CTRL_KI: bank = data_bus; break;
		case CTRL_MI: writeMemory(data_bus, address_out); break;
		case CTRL_CI: opcode = data_bus; break;
		case CTRL_TL: ((uint8_t *)&transfer_pointer)[0] = data_bus; break;
		case CTRL_TH: ((uint8_t *)&transfer_pointer)[1] = data_bus; break;
		case CTRL_ND: break;
	}

	// load opcode side effects
	if ((control & CTRL_ND) == CTRL_CI) {
		reset = 0;
		readInterrupts();
	}

	// increment step counter
	++step;
	if ((control & CTRL_ND) == CTRL_CI || step == 16 || reset == 1) step = 0;
}