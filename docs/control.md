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
| 0 - 2   | Register out to Data Bus <br> __AO__ Accumulator out <br> __FO__ Flags out <br> __KO__ Bank out <br> __MO__ Memory out <br> __PL/PH__ Pointer low/high byte out <br> __BL/BH__ Base pointer low/high byte out <br> __IL/IH__ Instruction Pointer low/high byte out <br> __HO__ High ($FF) out |
| 3       | Select between low and high bytes <br> (used for __PL/PH__, __BL/BH__, and __IL/IH__) |
| 4 - 6   | Register in from Data Bus <br> __AI__ Accumulator in <br> __XI__ Index in <br> __KI__ Bank in <br> __MI__ Memory in <br> __CI__ Instruction code in (indictes end of instruction) <br> __TL__ Transfer low byte in <br> __TH__ Transfer high byte in <br> None in form Data Bus |
| 7       | __FI__ Flags register in from source selected by __FB__ |
| 8 - 9   | Register out to Address Bus <br> __PO__ Pointer out <br> __BO__ Base pointer out <br> __SO__ Stack pointer out <br> __IO__ Instruction pointer out |
| 10      | __TO__ Transfer pointer out to Address Bus <br> (overrides choice of __PO__, __BO__, __SO__, or __IO__) |
| 11 - 12 | Register in from Address Bus <br> __PI__ Pointer in <br> __BI__ Base pointer in <br> __SI__ Stack pointer in <br> __II__ Instruction pointer in |
| 13      | None in from Address Bus <br> (overrides choice of __PI__, __BI__, __SI__, or __II__) |
| 14      | __FB__ Flags in from Data Bus, otherwise in from ALU    |
| 15      | __KZ__ Bank output is zeroed                            |
| 16      | __XZ__ Index output is zeroed                           |
| 17      | __XN__ Index output is negated (one's complement)       |
| 18      | __XC__ Index carry in                                   |
| 19      | __EZ__ ALU accumulator input is zero                    |
| 20      | __EN__ ALU data bus input is negated (one's complement) |
| 21      | __EC__ ALU carry in                                     |
| 22 - 23 | ALU Operation <br> __ES__ Sum inputs <br> __ER__ Right shift data bus input <br> __EA__ Logical AND inputs <br> __EO__ Logical OR inputs |