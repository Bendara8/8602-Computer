#include "draw.h"
#include "interface.h"
#include "input.h"
#include "interrupt.h"
#include "cpu.h"
#include "control.h"
#include "memory.h"
#include <ncurses.h>

static const char *opcodeToStr(uint8_t opcode);

void drawRegisters(int line) {
	const struct CPUState *cpu = getCPU();
	move(line + 1, 0);
	printw("A    : $%02hhX (%03hhu) (%+04hhd)\n", cpu->accumulator, cpu->accumulator, cpu->accumulator);
	printw("X    : $%02hhX (%03hhu) (%+04hhd)\n", cpu->index, cpu->index, cpu->index);
	printw("K    : $%02hhX\n", cpu->bank);
	printw("C    : $%02hhX (%s) RESET:%c INT0:%c INT1:%c\n",
		cpu->opcode,
		opcodeToStr(cpu->opcode),
		cpu->reset ? '1' : '0',
		getInterrupt(INTER_0) ? '1' : '0',
		getInterrupt(INTER_1) ? '1' : '0'
	);
	printw("F    : $%02hhX (%c%c%c%c)\n",
		(uint8_t)cpu->flags,
		cpu->flags & FLAG_I ? 'I' : '-',
		cpu->flags & FLAG_C ? 'C' : '-',
		cpu->flags & FLAG_N ? 'N' : '-',
		cpu->flags & FLAG_Z ? 'Z' : '-'
	);
	uint8_t byte = readMemory(cpu->transfer_pointer);
	uint16_t word = (readMemory(cpu->transfer_pointer + 1) << 8) | byte;
	printw("T    : $%04hX -> $%02hhX (%03hhu) (%+04hhd) | $%04hX (%05hu) (%+06hd)\n",
		cpu->transfer_pointer,
		byte, byte, byte, word, word, word
	);
	byte = readMemory(cpu->transfer_pointer + cpu->index);
	word = (readMemory(cpu->transfer_pointer + cpu->index + 1) << 8) | byte;
	printw("T+X  : $%04hX -> $%02hhX (%03hhu) (%+04hhd) | $%04hX (%05hu) (%+06hd)\n",
		cpu->transfer_pointer + cpu->index,
		byte, byte, byte, word, word, word
	);
	byte = readMemory(cpu->pointer);
	word = (readMemory(cpu->pointer + 1) << 8) | byte;
	printw("P    : $%04hX -> $%02hhX (%03hhu) (%+04hhd) | $%04hX (%05hu) (%+06hd)\n",
		cpu->pointer,
		byte, byte, byte, word, word, word
	);
	byte = readMemory(cpu->pointer + cpu->index);
	word = (readMemory(cpu->pointer + cpu->index + 1) << 8) | byte;
	printw("P+X  : $%04hX -> $%02hhX (%03hhu) (%+04hhd) | $%04hX (%05hu) (%+06hd)\n",
		cpu->pointer + cpu->index,
		byte, byte, byte, word, word, word
	);
	byte = readMemory(cpu->base_pointer);
	word = (readMemory(cpu->base_pointer + 1) << 8) | byte;
	printw("B    : $%04hX -> $%02hhX (%03hhu) (%+04hhd) | $%04hX (%05hu) (%+06hd)\n",
		cpu->base_pointer,
		byte, byte, byte, word, word, word
	);
	byte = readMemory(cpu->base_pointer + cpu->index);
	word = (readMemory(cpu->base_pointer + cpu->index + 1) << 8) | byte;
	printw("B+X  : $%04hX -> $%02hhX (%03hhu) (%+04hhd) | $%04hX (%05hu) (%+06hd)\n",
		cpu->base_pointer + cpu->index,
		byte, byte, byte, word, word, word
	);
	byte = readMemory(cpu->stack_pointer);
	word = (readMemory(cpu->stack_pointer + 1) << 8) | byte;
	printw("S    : $%04hX -> $%02hhX (%03hhu) (%+04hhd) | $%04hX (%05hu) (%+06hd)\n",
		cpu->stack_pointer,
		byte, byte, byte, word, word, word
	);
	byte = readMemory(cpu->instruction_pointer);
	word = (readMemory(cpu->instruction_pointer + 1) << 8) | byte;
	printw("I    : $%04hX -> $%02hhX (%03hhu) (%+04hhd) | $%04hX (%05hu) (%+06hd)\n",
		cpu->instruction_pointer,
		byte, byte, byte, word, word, word
	);
	static const char *ADDR_OUT[] = {
		"TO", "TO", "TO", "TO", "PO", "BO", "SO", "IO",
	};
	static const char *ADDR_IN[] = {
		"--", "--", "--", "--", "PI", "BI", "SI", "II",
	};
	static const char *DATA_OUT[] = {
		"AO", "FO", "KO", "MO", "PL", "BL", "IL", "HO", "??", "??", "??", "??", "PH", "BH", "IH", "??",
	};
	static const char *ALU_SEL[] = {
		"ES", "ER", "EA", "EO",
	};
	static const char *DATA_IN[] = {
		"AI", "XI", "KI", "MI", "CI", "TL", "TH", "ND",
	};
	printw("CTRL : $%06X (%s %s %s %s %s %s %s %s %s %s %s %s %s %s) STEP:%02hhu\n",
		cpu->control,
		ADDR_OUT[(cpu->control >> 8) & 0x7],
		cpu->control & CTRL_KZ ? "--" : "KZ",
		cpu->control & CTRL_XZ ? "--" : "XZ",
		cpu->control & CTRL_XN ? "XN" : "--",
		cpu->control & CTRL_XC ? "XC" : "--",
		ADDR_IN[(cpu->control >> 11) & 0x7],
		DATA_OUT[(cpu->control >> 0) & 0xF],
		cpu->control & CTRL_EZ ? "--" : "EZ",
		cpu->control & CTRL_EN ? "EN" : "--",
		cpu->control & CTRL_EC ? "EC" : "--",
		ALU_SEL[(cpu->control >> 22) & 0x3],
		DATA_IN[(cpu->control >> 4) & 0x7],
		cpu->control & CTRL_FI ? "FI" : "--",
		cpu->control & CTRL_FD ? "FD" : "--",
		cpu->step
	);
}

void drawCode(int line) {

}

void drawStack(int line) {

}

void drawMemory(int line) {

}

void drawFlash(int line) {

}

void drawConsole(int line) {
	move(LINES - 1, 0);
	printw("> %s", getInput());
	move(LINES - 2, 0);
	addstr(getFeedback());
}

const char *opcodeToStr(uint8_t opcode) {
	static const char *INS_TABLE[] = {
		"lod A #",   "lod A _", "lod A B+#", "lod A B-#", "lod A P+#", "lod A P+_", "lod A P+B+#", "lod A P+B-#",
		"sto P _",   "lod P _", "lod P B+#", "lod P B-#", "lod P P+#", "lod P P+_", "lod P P+B+#", "lod P P+B-#",
		"sto P B+#", "lea P _", "lea P B+#", "lea P B-#", "lea P P+#", "lea P P+_", "lea P P+B+#", "lea P P+B-#",
		"sto P B-#", "sto A _", "sto A B+#", "sto A B-#", "sto A P+#", "sto A P+_", "sto A P+B+#", "sto A P+B-#",
		"mov A F",   "mov F A", "mov P B",   "mov P S",   "mov B P",   "mov B S",   "mov S P",     "mov S B",
		"psh A",     "psh F",   "phw P",     "phw B",     "pul A",     "pul F",     "plw P",       "plw B",
		"psh #",     "psh _",   "psh B+#",   "psh B-#",   "psh P+#",   "psh P+_",   "psh P+B+#",   "psh P+B-#",
		"pop #",     "pul _",   "pul B+#",   "pul B-#",   "pul P+#",   "pul P+_",   "pul P+B+#",   "pul P+B-#",
		"phw #",     "phw _",   "phw B+#",   "phw B-#",   "phw P+#",   "phw P+_",   "phw P+B+#",   "phw P+B-#",
		"nop",       "plw _",   "plw B+#",   "plw B-#",   "plw P+#",   "plw P+_",   "plw P+B+#",   "plw P+B-#",
		"add #",     "add _",   "add B+#",   "add B-#",   "add P+#",   "add P+_",   "add P+B+#",   "add P+B-#",
		"adc #",     "adc _",   "adc B+#",   "adc B-#",   "adc P+#",   "adc P+_",   "adc P+B+#",   "adc P+B-#",
		"sub #",     "sub _",   "sub B+#",   "sub B-#",   "sub P+#",   "sub P+_",   "sub P+B+#",   "sub P+B-#",
		"sbc #",     "sbc _",   "sbc B+#",   "sbc B-#",   "sbc P+#",   "sbc P+_",   "sbc P+B+#",   "sbc P+B-#",
		"cmp #",     "cmp _",   "cmp B+#",   "cmp B-#",   "cmp P+#",   "cmp P+_",   "cmp P+B+#",   "cmp P+B-#",
		"asr A",     "asr _",   "asr B+#",   "asr B-#",   "asr P+#",   "asr P+_",   "asr P+B+#",   "asr P+B-#",
		"lsl A",     "lsl _",   "lsl B+#",   "lsl B-#",   "lsl P+#",   "lsl P+_",   "lsl P+B+#",   "lsl P+B-#",
		"lsr A",     "lsr _",   "lsr B+#",   "lsr B-#",   "lsr P+#",   "lsr P+_",   "lsr P+B+#",   "lsr P+B-#",
		"rol A",     "rol _",   "rol B+#",   "rol B-#",   "rol P+#",   "rol P+_",   "rol P+B+#",   "rol P+B-#",
		"ror A",     "ror _",   "ror B+#",   "ror B-#",   "ror P+#",   "ror P+_",   "ror P+B+#",   "ror P+B-#",
		"and #",     "and _",   "and B+#",   "and B-#",   "and P+#",   "and P+_",   "and P+B+#",   "and P+B-#",
		"ora #",     "ora _",   "ora B+#",   "ora B-#",   "ora P+#",   "ora P+_",   "ora P+B+#",   "ora P+B-#",
		"not A",     "not _",   "not B+#",   "not B-#",   "not P+#",   "not P+_",   "not P+B+#",   "not P+B-#",
		"neg A",     "neg _",   "neg B+#",   "neg B-#",   "neg P+#",   "neg P+_",   "neg P+B+#",   "neg P+B-#",
		"inc A",     "inc _",   "inc B+#",   "inc B-#",   "inc P+#",   "inc P+_",   "inc P+B+#",   "inc P+B-#",
		"dec A",     "dec _",   "dec B+#",   "dec B-#",   "dec P+#",   "dec P+_",   "dec P+B+#",   "dec P+B-#",
		"ict A",     "ict _",   "ict B+#",   "ict B-#",   "dct A",     "dct _",     "dct B+#",     "dct B-#",
		"inc P",     "dec P",   "ict P",     "dct P",     "set #",     "clr #",     "mov A K",     "mov K A",
		"brz I+#",   "brz I-#", "bnz I+#  ", "bnz I-#  ", "brc I+#",   "brc I-#",   "bnc I+#",     "bnc I-#",
		"brn I+#",   "brn I-#", "bnn I+#  ", "bnn I-#  ", "brp I+#",   "brp I-#",   "bnp I+#",     "bnp I-#",
		"bra I+#",   "bra I-#", "vec P+B+#", "vec P+B-#", "jmp _",     "jpl _",     "ent #",       "ext #",
		"jsr _",     "jsl _",   "rts",       "rts #",     "rtl",       "rtl #",     "rti",         "brk",
	};
	return INS_TABLE[opcode];
}