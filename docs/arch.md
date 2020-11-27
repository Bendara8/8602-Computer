# Architecture Documentation
- [Registers](#regs)
- [Flags](#flags)

<a name="regs"></a>
## Registers
| Register            | Mnenomic | Description                            |
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
| Flag      | Mnenomic | Description
| :--:      | :------: | -----------                                                            |
| Zero      | Z        | Set when the result of an ALU operation is zero                        |
| Negative  | N        | Set when the result of an ALU operation is negative (two's complement) |
| Carry     | C        | Carry out from a sum operation or shift out from a shift operation     |
| Interrupt | I        | Interrupt enable                                                       |
