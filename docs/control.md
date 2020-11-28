# 8602 Control
- [Architecture](./arch.md)
	- [Registers](./arch.md#regs)
	- [Flags](./arch.md#flags)
- [System](./system.md)
	- [Memory Map](./system.md#map)
	- [I/O Registers](./system.md#io)
- [Assembly Language](./assembly.md)
	- [Addressing Modes](./assembly.md#modes)
	- [Instruction Set](./assembly.md#set)
- [Control](./control.md)
	- [Control Inputs](./control.md#inputs)
	- [Control Outputs](./control.md#outputs)
	- [Microcode](./control.md#micro)

<a name="inputs"></a>
## Control Inputs
| Bits   | Description      |
| :--:   | -----------      |
| 0 - 3  | Step counter     |
| 4      | Zero flag        |
| 5      | Negative flag    |
| 6      | Carry flag       |
| 7      | Interrupt flag   |
| 8 - 15 | Instruction code |
| 16     | Interrupt signal |
| 17     | Reset signal     |

<a name="outputs"></a>
## Control Outputs
| Bits    | Description |
| :--:    | ----------- |
| 0 - 2   | Register out to data bus <br> `AO` Accumulator out <br> `FO` Flags out <br> `KO` Bank out <br> `MO` Memory out <br> `PL`/`PH` Pointer low/high byte out <br> `BL`/`BH` Base pointer low/high byte out <br> `IL`/`IH` Instruction Pointer low/high byte out <br> `HO` High ($FF) out |
| 3       | Select between low and high bytes <br> (used for `PL`/`PH`, `BL`/`BH`, and `IL`/`IH`) |
| 4 - 6   | Register in from data bus or ALU result bus <br> `AI` Accumulator in from ALU result bus <br> `XI` Index in from data bus <br> `KI` Bank in from data bus <br> `MI` Memory in from data bus <br> `CI` Opcode in from data bus (indicates end of instruction) <br> `TL` Transfer low byte in from data bus <br> `TH` Transfer high byte in from data bus <br> None in from data bus or ALU result bus |
| 7       | `FI` Flags register in from source selected by `FB` |
| 8 - 9   | Register out to address bus <br> `PO` Pointer out <br> `BO` Base pointer out <br> `SO` Stack pointer out <br> `IO` Instruction pointer out |
| 10      | `TO` Transfer pointer out to address bus <br> (overrides choice of `PO`, `BO`, `SO`, or `IO`) |
| 11 - 12 | Register in from indexed address bus <br> `PI` Pointer in <br> `BI` Base pointer in <br> `SI` Stack pointer in <br> `II` Instruction pointer in |
| 13      | None in from indexed address bus <br> (overrides choice of `PI`, `BI`, `SI`, or `II`) |
| 14      | `FD` Flags in from data bus, otherwise in from ALU    |
| 15      | `KZ` Bank output is zeroed                            |
| 16      | `XZ` Index output is zeroed                           |
| 17      | `XN` Index output is negated (one's complement)       |
| 18      | `XC` Index carry in                                   |
| 19      | `EZ` ALU accumulator input is zero                    |
| 20      | `EN` ALU data bus input is negated (one's complement) |
| 21      | `EC` ALU carry in or shift in                         |
| 22 - 23 | ALU Operation <br> `ES` Sum inputs (updates zero, negative, and carry flags) <br> `ER` Right shift data bus input <br> `EA` Logical AND inputs <br> `EO` Logical OR inputs |

<a name="micro"></a>
## Microcode
Each 8602 instruction consists of smaller instructions within, known as microcode. The microcode file for the 8602 can be found [here](../microcode/mircocode.src). This file uses a custom syntax for defining the microcode of each instruction, outlined below.

Here is an example for one of the simplest instructions in the 8602 instruction set, the `lod A #imm` instruction.
```
$00: lod A #imm
	IO -- XZ -- XC II MO EZ -- -- EO AI FI --
	IO -- XZ -- XC II MO -- -- -- EO CI -- --
```
The microcode begins with the opcode for the `lod A #imm`, which is `$00`. Then the name of the instruction is listed after a comma, and the mircocode for the instruction is listed on the following lines, with each line representing a subsequent time step. A mircocode word is composed entirely of control output mnemonics, indicating which ones are active. The first mircocode word can be broken down as follows.

- `IO -- XZ -- XC II MO AI EZ EO FI`
	- `IO` Output the instruction pointer to the address bus
	- `XZ` The index output is zeroed
	- `XC` The index offset carry input is set
	- `II` Load the instruction pointer from the indexed address bus
	- `MO` Output the memory contents of the indexed address to the data bus
	- `AI` Load accumulator from ALU result bus
	- `EZ` Zero accumulator input to ALU
	- `EO` Select bitwise OR operation for ALU
	- `FI` Load negative and zero flags from ALU

In short, this microcode word will increment the instruction pointer and load the accumulator with the contents of the incremented instruction pointer while also updating the negative and zero flags appropriately. The next microcode word is very similar, differing only in which register loads from memory. In this case, the opcode register is loaded instead of the accumulator. Notice that even though the ALU is not being used in this time step, an operation still must be selected. The OR operation is chosen by convention in this situation. This microcode word is the standard fetch cycle which is present at the end of most 8602 instructions.

The dashes `--` are used to indicate control outputs which are not active for that microcode word. In the above example they represent the `KZ` and `XN` control outputs, which are not active during the time steps that they are present in. The structure of a microcode word is standardized and each part of the word can be filled with certain control outputs, or in some cases (such as `KZ` and `XN`) left blank using dashes. The structure of a microcode word is defined as follows.

| Part                                             | Options
| :--:                                             | -------
| Register out to address bus                      | `PO` `BO` `SO` `IO` `TO`
| Bank output is zeroed                            | `KZ` `--`
| Index output is zeroed                           | `XZ` `--`
| Index output is negated (one's complement)       | `XN` `--`
| Index carry in                                   | `XC` `--`
| Register in from indexed address bus             | `PI` `BI` `SI` `II` `--`
| Register out to data bus                         | `AO` `FO` `KO` `MO` `PL` `PH` `BL` `BH` `IL` `IH` `HO`
| ALU accumulator input is zeroed                  | `EZ` `--`
| ALU data bus input is negated (one's complement) | `EN` `--`
| ALU carry in or shift in                         | `EC` `--`
| ALU operation select                             | `ES` `ER` `EA` `EO`
| Register in from data bus or ALU result bus      | `AI` `XI` `KI` `MI` `CI` `TL` `TH` `--`
| Flags in                                         | `FI` `--`
| Flags in from data bus instead of ALU            | `FD` `--`

Some instructions require different mircrocode words depending on the state of the flags register. A simple branching syntax is used in these situations. Here is an example of the branching syntax in the `brz I+#off` instruction.
```
E0: brz I+#off
	(Z) {
	IA -- XZ -- XC II MO -- -- -- EO XI -- --
	IA -- -- -- XC II MO -- -- -- EO CI -- --
	} {
	IA -- XZ -- XC II MO -- -- -- EO -- -- --
	IA -- XZ -- XC II MO -- -- -- EO CI -- --
	}
```
If the condition matches the current state of the flags, then the top branch will be taken, otherwise the bottom branch is taken. If the lengths of the two branches are not the same their lengths will be matched by filling the shorter with non-operational microcode words. The flags will match if any of the flags inside the parentheses are set. The condition `(Z)` will match if the zero flag is set, regardless of any other flags. The condition `(ZN)` will match if either the zero flag or the negative flag are set.