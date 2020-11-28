# 8602 Architecture
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

<a name="diagram"></a>
## Diagram
Here is the 8602 architecture block diagram. Arrows pointing into blocks indicate that data can be latched into the block from that bus. Arrows pointing into a bus from a block indicate that data can be output from the block onto that bus.
![8602 Architecture Block Diagram](./res/arch-diagram.svg)

<a name="regs"></a>
## Registers
| Register            | Mnemonic | Description                            |
| :------:            | :------: | -----------                            |
| Accumlator          | A        | General purpose data and ALU result    |
| Index               | X        | Address offset                         |
| Flags               | F        | Status flags for conditional branching |
| Bank                | K        | Current flash memory bank              |
| Opcode              | C        | Currently executing opcode             |
| Pointer             | P        | General purpose address pointer        |
| Base Pointer        | B        | Pointer to base of stack frame         |
| Stack Pointer       | S        | Pointer to top of stack                |
| Instruction Pointer | I        | Pointer to current instruction         |

<a name="flags"></a>
## Flags
| Flag      | Mnemonic | Description
| :--:      | :------: | -----------                                                            |
| Zero      | Z        | Set when the result of an ALU operation is zero                        |
| Negative  | N        | Set when the result of an ALU operation is negative (two's complement) |
| Carry     | C        | Carry out from a sum operation or shift out from a shift operation     |
| Interrupt | I        | Interrupt enable                                                       |

