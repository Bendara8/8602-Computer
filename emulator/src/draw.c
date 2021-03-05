#include "draw.h"
#include "command.h"
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
	printw("A : $%02hhX (%03hhu) (%+04hhd)\n", cpu->accumulator, cpu->accumulator, cpu->accumulator);
	printw("X : $%02hhX (%03hhu) (%+04hhd)\n", cpu->index, cpu->index, cpu->index);
	printw("K : $%02hhX\n", cpu->bank);
	printw("F : $%02hhX (%c%c%c%c)\n",
		(uint8_t)cpu->flags,
		cpu->flags & FLAG_I ? 'I' : '-',
		cpu->flags & FLAG_C ? 'C' : '-',
		cpu->flags & FLAG_N ? 'N' : '-',
		cpu->flags & FLAG_Z ? 'Z' : '-'
	);
	printw("C : $%02hhX (%s)\n",
		cpu->opcode,
		opcodeToStr(cpu->opcode)
	);
	uint8_t byte = readMemory(cpu->transfer_pointer);
	uint16_t word = (readMemory(cpu->transfer_pointer + 1) << 8) | byte;
	printw("T : $%04hX -> $%02hhX (%03hhu) (%+04hhd) | $%04hX (%05hu) (%+06hd)\n",
		cpu->transfer_pointer,
		byte, byte, byte, word, word, word
	);
	byte = readMemory(cpu->pointer);
	word = (readMemory(cpu->pointer + 1) << 8) | byte;
	printw("P : $%04hX -> $%02hhX (%03hhu) (%+04hhd) | $%04hX (%05hu) (%+06hd)\n",
		cpu->pointer,
		byte, byte, byte, word, word, word
	);
	byte = readMemory(cpu->base_pointer);
	word = (readMemory(cpu->base_pointer + 1) << 8) | byte;
	printw("B : $%04hX -> $%02hhX (%03hhu) (%+04hhd) | $%04hX (%05hu) (%+06hd)\n",
		cpu->base_pointer,
		byte, byte, byte, word, word, word
	);
	byte = readMemory(cpu->stack_pointer);
	word = (readMemory(cpu->stack_pointer + 1) << 8) | byte;
	printw("S : $%04hX -> $%02hhX (%03hhu) (%+04hhd) | $%04hX (%05hu) (%+06hd)\n",
		cpu->stack_pointer,
		byte, byte, byte, word, word, word
	);
	byte = readMemory(cpu->instruction_pointer);
	word = (readMemory(cpu->instruction_pointer + 1) << 8) | byte;
	printw("I : $%04hX -> $%02hhX (%03hhu) (%+04hhd) | $%04hX (%05hu) (%+06hd)\n",
		cpu->instruction_pointer,
		byte, byte, byte, word, word, word
	);
	printw("  : RESET:%c INT0:%c INT1:%c STEP:%02hhu\n",
		cpu->reset ? '1' : '0',
		getInterrupt(INTER_0) ? '1' : '0',
		getInterrupt(INTER_1) ? '1' : '0',
		cpu->step
	);
	static const char *ADDR_OUT[] = {
		"TO", "TO", "TO", "TO", "PO", "BO", "SO", "IO",
	};
	static const char *ADDR_IN[] = {
		"--", "--", "--", "--", "PI", "BI", "SI", "II",
	};
	static const char *DATA_OUT[] = {
		"AO", "FO", "KO", "MO", "PL", "BL", "IL", "ZO", "??", "??", "??", "??", "PH", "BH", "IH", "HO",
	};
	static const char *ALU_SEL[] = {
		"ES", "ER", "EA", "EO",
	};
	static const char *DATA_IN[] = {
		"AI", "XI", "KI", "MI", "CI", "TL", "TH", "--",
	};
	enum Control control = readControl(
		cpu->step, (uint8_t)cpu->flags, cpu->opcode,
		getInterrupt(INTER_0), getInterrupt(INTER_1),
		cpu->reset
	);
	printw("  : $%06X (%s %s %s %s %s %s %s %s %s %s %s %s %s)\n",
		control,
		ADDR_OUT[(control >> 8) & 0x7],
		control & CTRL_XZ ? "--" : "XZ",
		control & CTRL_XN ? "XN" : "--",
		control & CTRL_XC ? "XC" : "--",
		ADDR_IN[(control >> 11) & 0x7],
		DATA_OUT[(control >> 0) & 0xF],
		control & CTRL_EZ ? "--" : "EZ",
		control & CTRL_EN ? "EN" : "--",
		control & CTRL_EC ? "EC" : "--",
		ALU_SEL[(control >> 22) & 0x3],
		DATA_IN[(control >> 5) & 0x7],
		control & CTRL_FI ? "--" : "FI",
		control & CTRL_FD ? "FD" : "--"
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
	move(LINES - 2, 0);
	addstr(getFeedback());
	move(LINES - 1, 0);
	printw("> %s", getInput());
}

void drawConsoleLine(void) {
	move(LINES - 1, 0);
	printw("> %s", getInput());
	clrtoeol();
	refresh();
}

const char *opcodeToStr(uint8_t opcode) {
	static const char *INS_TABLE[] = {
		//    0             1             2               3               4             5             6               7
		//    8             9             A               B               C             D             E               F
		/*0*/ "lod #imm",   "lod adr",    "lod B+#imm",   "lod B-#imm",   "lod P+#imm", "lod P+adr",  "lod P+B+#imm", "lod P+B-#imm",
		/* */ "stw adr",    "ldw adr",    "ldw B+#imm",   "ldw B-#imm",   "ldw P+#imm", "ldw P+adr",  "ldw P+B+#imm", "ldw P+B-#imm",
		/*1*/ "stw B+#imm", "lea adr",    "lea B+#imm",   "lea B-#imm",   "lea P+#imm", "lea P+adr",  "lea P+B+#imm", "lea P+B-#imm",
		/* */ "stw B-#imm", "sto adr",    "sto B+#imm",   "sto B-#imm",   "sto P+#imm", "sto P+adr",  "sto P+B+#imm", "sto P+B-#imm",
		/*2*/ "mov A F",    "mov F A",    "mov P B",      "mov P S",      "mov B P",    "mov B S",    "mov S P",      "mov S B",
		/* */ "psh A",      "psh F",      "phw P",        "phw B",        "pul A",      "pul F",      "plw P",        "plw B",
		/*3*/ "psh #imm",   "psh adr",    "psh B+#imm",   "psh B-#imm",   "psh P+#imm", "psh P+adr",  "psh P+B+#imm", "psh P+B-#imm",
		/* */ "cop",        "phw adr",    "phw B+#imm",   "phw B-#imm",   "phw P+#imm", "phw P+adr",  "phw P+B+#imm", "phw P+B-#imm",
		/*4*/ "cpw",        "pea adr",    "pea B+#imm",   "pea B-#imm",   "pea P+#imm", "pea P+adr",  "pea P+B+#imm", "pea P+B-#imm",
		/* */ "        ",   "pul adr",    "pul B+#imm",   "pul B-#imm",   "pul P+#imm", "pul P+adr",  "pul P+B+#imm", "pul P+B-#imm",
		/*5*/ "nop",        "plw adr",    "plw B+#imm",   "plw B-#imm",   "plw P+#imm", "plw P+adr",  "plw P+B+#imm", "plw P+B-#imm",
		/* */ "not A",      "not adr",    "not B+#imm",   "not B-#imm",   "not P+#imm", "not P+adr",  "not P+B+#imm", "not P+B-#imm",
		/*6*/ "and #imm",   "and adr",    "and B+#imm",   "and B-#imm",   "and P+#imm", "and P+adr",  "and P+B+#imm", "and P+B-#imm",
		/* */ "ora #imm",   "ora adr",    "ora B+#imm",   "ora B-#imm",   "ora P+#imm", "ora P+adr",  "ora P+B+#imm", "ora P+B-#imm",
		/*7*/ "add #imm",   "add adr",    "add B+#imm",   "add B-#imm",   "add P+#imm", "add P+adr",  "add P+B+#imm", "add P+B-#imm",
		/* */ "adc #imm",   "adc adr",    "adc B+#imm",   "adc B-#imm",   "adc P+#imm", "adc P+adr",  "adc P+B+#imm", "adc P+B-#imm",
		/*8*/ "sub #imm",   "sub adr",    "sub B+#imm",   "sub B-#imm",   "sub P+#imm", "sub P+adr",  "sub P+B+#imm", "sub P+B-#imm",
		/* */ "sbc #imm",   "sbc adr",    "sbc B+#imm",   "sbc B-#imm",   "sbc P+#imm", "sbc P+adr",  "sbc P+B+#imm", "sbc P+B-#imm",
		/*9*/ "cmp #imm",   "cmp adr",    "cmp B+#imm",   "cmp B-#imm",   "cmp P+#imm", "cmp P+adr",  "cmp P+B+#imm", "cmp P+B-#imm",
		/* */ "asr A",      "asr adr",    "asr B+#imm",   "asr B-#imm",   "asr P+#imm", "asr P+adr",  "asr P+B+#imm", "asr P+B-#imm",
		/*A*/ "lsl A",      "lsl adr",    "lsl B+#imm",   "lsl B-#imm",   "lsl P+#imm", "lsl P+adr",  "lsl P+B+#imm", "lsl P+B-#imm",
		/* */ "lsr A",      "lsr adr",    "lsr B+#imm",   "lsr B-#imm",   "lsr P+#imm", "lsr P+adr",  "lsr P+B+#imm", "lsr P+B-#imm",
		/*B*/ "rol A",      "rol adr",    "rol B+#imm",   "rol B-#imm",   "rol P+#imm", "rol P+adr",  "rol P+B+#imm", "rol P+B-#imm",
		/* */ "ror A",      "ror adr",    "ror B+#imm",   "ror B-#imm",   "ror P+#imm", "ror P+adr",  "ror P+B+#imm", "ror P+B-#imm",
		/*C*/ "inc A",      "inc adr",    "inc B+#imm",   "inc B-#imm",   "inc P+#imm", "inc P+adr",  "inc P+B+#imm", "inc P+B-#imm",
		/* */ "dec A",      "dec adr",    "dec B+#imm",   "dec B-#imm",   "dec P+#imm", "dec P+adr",  "dec P+B+#imm", "dec P+B-#imm",
		/*D*/ "ict A",      "ict adr",    "ict B+#imm",   "ict B-#imm",   "dct A",      "dct adr",    "dct B+#imm",   "dct B-#imm",
		/* */ "inc P",      "dec P",      "ict P",        "dct P",        "set #imm",   "clr #imm",   "mov A K",      "mov K A",
		/*E*/ "brz I+#imm", "brz I-#imm", "bnz I+#imm  ", "bnz I-#imm  ", "brc I+#imm", "brc I-#imm", "bnc I+#imm",   "bnc I-#imm",
		/* */ "brn I+#imm", "brn I-#imm", "bnn I+#imm  ", "bnn I-#imm  ", "brg I+#imm", "brg I-#imm", "bng I+#imm",   "bng I-#imm",
		/*F*/ "bra I+#imm", "bra I-#imm", "vec P+B+#imm", "vec P+B-#imm", "jmp adr",    "jpl adr",    "ent #imm",     "ext #imm",
		/* */ "jsr adr",    "jsl adr",    "rts",          "rts #imm",     "rtl",        "rtl #imm",   "rti",          "brk",
	};
	return INS_TABLE[opcode];
}