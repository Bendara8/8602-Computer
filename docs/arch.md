# 8602 Architecture
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

<a name="regs"></a>
## Registers
| Register            | Mnemonic | Description                            |
| :------:            | :------: | -----------                            |
| Accumlator          | A        | General purpose data and ALU result    |
| Index               | X        | Address offset                         |
| Flags               | F        | Status flags for conditional branching |
| Bank                | K        | Current flash memory bank              |
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

