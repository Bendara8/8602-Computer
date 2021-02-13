#include "cpu.h"
#include "interrupt.h"
#include "alu.h"
#include "control.h"
#include "memory.h"

static struct CPUState cpu;

void resetCPU(void) {
	cpu.step = 0;
	cpu.reset = 1;
}

void clockCPU(void) {
	// get control word
	cpu.control = readControl(
		cpu.step, (uint8_t)cpu.flags, cpu.opcode,
		getInterrupt(INTER_0), getInterrupt(INTER_1),
		cpu.reset
	);

	// output to address bus
	uint16_t address_bus = 0;
	if (!(cpu.control & CTRL_TO)) address_bus = cpu.transfer_pointer;
	else switch (cpu.control & CTRL_IO) {
		case CTRL_PO: address_bus = cpu.pointer; break;
		case CTRL_BO: address_bus = cpu.base_pointer; break;
		case CTRL_SO: address_bus = cpu.stack_pointer; break;
		case CTRL_IO: address_bus = cpu.instruction_pointer; break;
	}

	// index address
	uint16_t index_out = cpu.index;
	if (!(cpu.control & CTRL_XZ)) index_out = 0;
	if (cpu.control & CTRL_XN) index_out = ~index_out;
	uint16_t address_out = address_bus + index_out + (cpu.control & CTRL_XC ? 1 : 0);

	// output to data bus
	uint8_t data_bus = 0;
	switch (cpu.control & CTRL_OD) {
		case CTRL_AO: data_bus = cpu.accumulator; break;
		case CTRL_FO: data_bus = (uint8_t)cpu.flags; break;
		case CTRL_KO: data_bus = cpu.bank; break;
		case CTRL_MO: data_bus = readMemory(address_out); break;
		case CTRL_PL: data_bus = (uint8_t)cpu.pointer; break;
		case CTRL_PH: data_bus = (uint8_t)(cpu.pointer >> 8); break;
		case CTRL_BL: data_bus = (uint8_t)cpu.base_pointer; break;
		case CTRL_BH: data_bus = (uint8_t)(cpu.base_pointer >> 8); break;
		case CTRL_IL: data_bus = (uint8_t)cpu.instruction_pointer; break;
		case CTRL_IH: data_bus = (uint8_t)(cpu.instruction_pointer >> 8); break;
		case CTRL_HO: data_bus = 0xFF; break;
	}

	// calculate alu
	uint8_t alu_out = calcALU(cpu.accumulator, data_bus, &cpu.flags, cpu.control);

	// load from address output
	if (cpu.control & CTRL_NA) switch (cpu.control & CTRL_II) {
		case CTRL_PI: cpu.pointer = address_out; break;
		case CTRL_BI: cpu.base_pointer = address_out; break;
		case CTRL_SI: cpu.stack_pointer = address_out; break;
		case CTRL_II: cpu.instruction_pointer = address_out; break;
	}

	// load from data bus and alu
	if (!(cpu.control & CTRL_FI) && cpu.control & CTRL_FD) cpu.flags = data_bus;
	switch (cpu.control & CTRL_ND) {
		case CTRL_AI: cpu.accumulator = alu_out; break;
		case CTRL_XI: cpu.index = data_bus; break;
		case CTRL_KI: cpu.bank = data_bus; setBank(cpu.bank); break;
		case CTRL_MI: writeMemory(data_bus, address_out); break;
		case CTRL_CI: cpu.opcode = data_bus; break;
		case CTRL_TL: ((uint8_t *)&cpu.transfer_pointer)[0] = data_bus; break;
		case CTRL_TH: ((uint8_t *)&cpu.transfer_pointer)[1] = data_bus; break;
		case CTRL_ND: break;
	}

	// load opcode side effects
	if ((cpu.control & CTRL_ND) == CTRL_CI) {
		cpu.reset = 0;
		readInterrupts();
	}

	// increment step counter
	++cpu.step;
	if ((cpu.control & CTRL_ND) == CTRL_CI || cpu.step == 16) cpu.step = 0;
}

const struct CPUState *getCPU(void) {
	return &cpu;
}