# 8602 Assembly Language
- [Architecture](./arch.md)
	- [Diagram](./arch.md#diagram)
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

<a name="modes"></a>
## Addressing Modes
| Mode               | Syntax                     | Description                                                   |
| :--:               | :----:                     | -----------                                                   |
| Register           | `reg`                      | Inside register                                               |
| Immediate          | `#imm`                     | Immediately after instruction                                 |
| Absolute           | `adr`                      | At an address                                                 |
| Instruction Offset | `I+#imm` <br> `I-#imm`     | At an offset to the instruction pointer                       |
| Base Offset        | `B+#imm` <br> `B-#imm`     | At an offset to the base pointer                              |
| Immediate Index    | `P+#imm`                   | At the pointer with an immediate index                        | 
| Absolute Index     | `P+adr`                    | At the pointer with an index at the address                   | 
| Offset Index       | `P+B+#imm` <br> `P+B-#imm` | At the pointer with an index at an offset to the base pointer | 

<a name="dir"></a>
## Directives
| Directive             | Description                   |
| :-------:             | -----------                   |
| [`include`](#include) | Include another assembly file |
| [`origin`](#origin)   | Set origin of following code  |
| [`symbol`](#symbol)   | Define a symbol               |
| [`data`](#data)       | Insert raw data               |

<a name="set"></a>
## Instruction Set
### Data Movement
| Mnemonic      | Description                                                                 |
| :------:      | -----------                                                                 |
| [`lod`](#lod) | Load a register with a value                                                |
| [`lea`](#lea) | Load a register with the effective address of a value                       |
| [`sto`](#sto) | Store a register to a memory address                                        |
| [`mov`](#mov) | Move one register into another                                              |
| [`psh`](#psh) | Push a register or value onto the stack                                     |
| [`phw`](#phw) | Push a wide (16-bit) register or value onto the stack                       |
| [`pul`](#pul) | Pull a value from the stack into a register or a memory address             |
| [`plw`](#plw) | Pull a wide (16-bit) value from the stack into a register or memory address |
| [`pop`](#pop) | Pop a given number of items off of the stack                                |
| [`ent`](#ent) | Enter a stack frame of a given size                                         |
| [`ext`](#ext) | Exit a stack frame of a given size                                          |

### Arithmetic and Logic
| Mnemonic      | Description                                                    |
| :------:      | -----------                                                    |
| [`nop`](#nop) | No operation                                                   |
| [`add`](#add) | Add a value to the accumulator                                 |
| [`adc`](#adc) | Add a value to the accumulator with carry                      |
| [`sub`](#sub) | Subtract a value from the accumulator                          |
| [`sbc`](#sbc) | Subtract a value from the accumulator with carry               |
| [`cmp`](#cmp) | Compare a value to the accumulator                             |
| [`inc`](#inc) | Increment a register or a memory address                       |
| [`ict`](#ict) | Increment a register or a memory address twice                 |
| [`dec`](#dec) | Decrement a register or a memory address                       |
| [`dct`](#dct) | Decrement a register or a memory address twice                 |
| [`neg`](#neg) | Negate a register or a memory address                          |
| [`and`](#and) | Bitwise AND a value with the accumulator                       |
| [`ora`](#ior) | Bitwise OR a value with the accumulator                        |
| [`not`](#not) | Bitwise NOT a register or a memory address                     |
| [`asr`](#asr) | Arithmetically shift a register or memory address to the right |
| [`lsl`](#lsl) | Logically shift a register or memory address to the left       |
| [`lsr`](#lsr) | Logically shift a register or memory address to the right      |
| [`rol`](#rol) | Rotate a register or memory address to the left                |
| [`ror`](#ror) | Rotate a register or memory address to the right               |
| [`set`](#set) | Set flags                                                      |
| [`clr`](#clr) | Clear flags                                                    |

### Control Flow
| Mnemonic      | Description                                                  |
| :------:      | -----------                                                  |
| [`bra`](#bra) | Branch to instruction pointer offset                         |
| [`brz`](#brz) | Branch if the zero flag is set                               |
| [`bnz`](#bnz) | Branch if the zero flag is clear                             |
| [`brn`](#brn) | Branch if the negative flag is set                           |
| [`bnn`](#bnn) | Branch if the negative flag is clear                         |
| [`brp`](#brp) | Branch if both the zero flag and the negative flag are clear |
| [`bnp`](#bnp) | Branch if either the zero flag or the negative flag are set  |
| [`brc`](#brc) | Branch if the carry flag is set                              |
| [`bnc`](#bnc) | Branch if the carry flag is clear                            |
| [`jmp`](#jmp) | Jump to address                                              |
| [`jpl`](#jpl) | Jump to long address                                         |
| [`jsr`](#jsr) | Jump to subroutine at address                                |
| [`jsl`](#jsl) | Jump to subroutine at long address                           |
| [`rts`](#rts) | Return from subroutine                                       |
| [`rtl`](#rtl) | Return from subroutine using long address                    |
| [`rti`](#rti) | Return from interrupt                                        |
| [`brk`](#brk) | Break execution                                              |

## Directives
<a name="include"></a>
### `include`

<a name="origin"></a>
### `origin`

<a name="symbol"></a>
### `symbol`

<a name="data"></a>
### `data`

## Instructions
<a name="lod"></a>
### `lod`
Loads the left operand with the right operand. When the left operand is the accumulator, the zero flag and the negative flag are set depending on the value loaded. When the left operand is the pointer register, only the zero flag is set correctly while negative flag is set arbitrarily.

| Opcode | Syntax           | Flags Affected |
| :----: | :----:           | -------------- |
| `$00`  | `lod A #imm`     | `Z` `N`        |
| `$01`  | `lod A adr`      | `Z` `N`        |
| `$02`  | `lod A B+#imm`   | `Z` `N`        |
| `$03`  | `lod A B-#imm`   | `Z` `N`        |
| `$04`  | `lod A P+#imm`   | `Z` `N`        |
| `$05`  | `lod A P+adr`    | `Z` `N`        |
| `$06`  | `lod A P+B+#imm` | `Z` `N`        |
| `$07`  | `lod A P+B-#imm` | `Z` `N`        |
| `$09`  | `lod P adr`      | `Z` `N`        |
| `$0A`  | `lod P B+#imm`   | `Z` `N`        |
| `$0B`  | `lod P B-#imm`   | `Z` `N`        |
| `$0C`  | `lod P P+#imm`   | `Z` `N`        |
| `$0D`  | `lod P P+adr`    | `Z` `N`        |
| `$0E`  | `lod P P+B+#imm` | `Z` `N`        |
| `$0F`  | `lod P P+B-#imm` | `Z` `N`        |

<a name="lea"></a>
### `lea`
Loads the left operand with the effective address of the right operand.

| Opcode | Syntax           | Flags Affected |
| :----: | :----:           | -------------- |
| `$11`  | `lea P adr`      |                |
| `$12`  | `lea P B+#imm`   |                |
| `$13`  | `lea P B-#imm`   |                |
| `$14`  | `lea P P+#imm`   |                |
| `$15`  | `lea P P+adr`    |                |
| `$16`  | `lea P P+B+#imm` |                |
| `$17`  | `lea P P+B-#imm` |                |

<a name="sto"></a>
### `sto`
Stores the left operand into the right operand.

| Opcode | Syntax           | Flags Affected |
| :----: | :----:           | -------------- |
| `$19`  | `sto A adr`      |                |
| `$1A`  | `sto A B+#imm`   |                |
| `$1B`  | `sto A B-#imm`   |                |
| `$1C`  | `sto A P+#imm`   |                |
| `$1D`  | `sto A P+adr`    |                |
| `$1E`  | `sto A P+B+#imm` |                |
| `$1F`  | `sto A P+B-#imm` |                |
| `$08`  | `sto P adr`      |                |
| `$10`  | `sto P B+#imm`   |                |
| `$18`  | `sto P B-#imm`   |                |

<a name="mov"></a>
### `mov`

<a name="psh"></a>
### `psh`

<a name="phw"></a>
### `phw`

<a name="pul"></a>
### `pul`

<a name="plw"></a>
### `plw`

<a name="pop"></a>
### `pop`

<a name="ent"></a>
### `ent`

<a name="ext"></a>
### `ext`

<a name="nop"></a>
### `nop`

<a name="add"></a>
### `add`

<a name="adc"></a>
### `adc`

<a name="sub"></a>
### `sub`

<a name="sbc"></a>
### `sbc`

<a name="cmp"></a>
### `cmp`

<a name="inc"></a>
### `inc`

<a name="ict"></a>
### `ict`

<a name="dec"></a>
### `dec`

<a name="dct"></a>
### `dct`

<a name="neg"></a>
### `neg`

<a name="and"></a>
### `and`

<a name="ora"></a>
### `ora`

<a name="not"></a>
### `not`

<a name="asr"></a>
### `asr`

<a name="lsl"></a>
### `lsl`

<a name="lsr"></a>
### `lsr`

<a name="rol"></a>
### `rol`

<a name="ror"></a>
### `ror`

<a name="set"></a>
### `set`

<a name="clr"></a>
### `clr`

<a name="bra"></a>
### `bra`

<a name="brz"></a>
### `brz`

<a name="bnz"></a>
### `bnz`

<a name="brn"></a>
### `brn`

<a name="bnn"></a>
### `bnn`

<a name="brp"></a>
### `brp`

<a name="bnp"></a>
### `bnp`

<a name="brc"></a>
### `brc`

<a name="bnc"></a>
### `bnc`

<a name="jmp"></a>
### `jmp`

<a name="jpl"></a>
### `jpl`

<a name="jsr"></a>
### `jsr`

<a name="jsl"></a>
### `jsl`

<a name="rts"></a>
### `rts`

<a name="rtl"></a>
### `rtl`

<a name="rti"></a>
### `rti`

<a name="brk"></a>
### `brk`

