# 8602-Computer
Tools, Simulator, CAD, and Documentation for the 8602 Discreet Logic Computer

## About
The 8602 is a minimalistic computer that is composed entirely of simple discreet logic chips from the 74HC family. Despite its simplicity, it features a VGA output and a PS/2 keyboard input.

### Specs
- 5 MHz clock rate
- Two display modes through an 800x600 SVGA signal
	- 400x256 pixel 2 color text mode
	- 200x128 pixel 16 color graphics mode
- 32 kB VRAM
	- Split into dual 16 kB buffers
- 16 kB SRAM with memory-mapped I/O
- Up to 4 MB of flash memory
	- Split into 32 kB banks
	- Minimum 500 kB for a single flash chip

## Documentation
### Registers
| Register            | Mnenomic | Description                          |
| :------:            | :------: | -----------                          |
| Accumlator          | A        | General purpse data and ALU result   |
| Index               | X        | Address offset                       |
| Flags               | F        | Status flags for conditional jumping |
| Bank                | K        | Current flash memory bank            |
| Pointer             | P        | General purpose address pointer      |
| Base Pointer        | B        | Pointer to base of stack frame       |
| Stack Pointer       | S        | Pointer to top of stack              |
| Instruction Pointer | I        | Pointer to current instruction       |

### Flags
| Flag      | Mnenomic | Description
| :--:      | :------: | -----------                                                            |
| Zero      | Z        | Set when the result of an ALU operation is zero                        |
| Negative  | N        | Set when the result of an ALU operation is negative (two's complement) |
| Carry     | C        | Carry out from a sum operation or shift out from a shift operation     |
| Interrupt | I        | Interrupt enable                                                       |

### Memory Map
| Address Range | Description                  |
| :-----------: | -----------                  |
| $0000 - $3FFF | Current VRAM buffer          |
| $4000 - $7EFF | General purpose SRAM         |
| $7F00 - $7FFF | I/O Page                     |
| $8000 - $FFFF | Current bank of flash memory |

### Control Inputs
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

### Control Outputs
| Bits    | Description                                                               |
| :--:    | -----------                                                               |
| 0 - 2   | Register out to Data Bus <br> __AO__ Accumulator out <br> __FO__ Flags out <br> __KO__ Bank out <br> __MO__ Memory out <br> __PL/PH__ Pointer low/high byte out <br> __BL/BH__ Base pointer low/high byte out <br> __IL/IH__ Instruction Pointer low/high byte out <br> __HO__ High ($FF) out |
| 3       | Select between low and high bytes for __PL/PH__, __BL/BH__, and __IL/IH__ |
| 4 - 6   | Register in from Data Bus <br> __AI__ Accumulator in <br> __XI__ Index in <br> __KI__ Bank in <br> __MI__ Memory in <br> __CI__ Instruction code in (indictes end of instruction) <br> __TL__ Transfer low byte in <br> __TH__ Transfer high byte in <br> None in form Data Bus |
| 7       | __FI__ Flags register in from source selected by __FB__                   |
| 8 - 9   | Register out to Address Bus <br> __PO__ Pointer out <br> __BO__ Base pointer out <br> __SO__ Stack pointer out <br> __IO__ Instruction pointer out |
| 10      | __TO__ Transfer pointer out to Address Bus <br> (overrides choice of __PO__, __BO__, __SO__, or __IO__) |
| 11 - 12 | Register in from Address Bus <br> __PI__ Pointer in <br> __BI__ Base pointer in <br> __SI__ Stack pointer in <br> __II__ Instruction pointer in |
| 13      | None in from Address Bus <br> (overrides choice of __PI__, __BI__, __SI__, or __II__) |
| 14      | __FB__ Flags in from Data Bus, otherwise in from ALU                      |
| 15      | __KZ__ Bank output is zeroed                                              |
| 16      | __XZ__ Index output is zeroed                                             |
| 17      | __XN__ Index output is negated (one's complement)                         |
| 18      | __XC__ Index carry in                                                     |
| 19      | __EZ__ ALU accumulator input is zero                                      |
| 20      | __EN__ ALU data bus input is negated (one's complement)                   |
| 21      | __EC__ ALU carry in                                                       |
| 22      | ALU Operation <br> __ES__ Sum inputs <br> __ER__ Right shift data bus input <br> __EA__ Logical AND inputs <br> __EO__ Logical OR inputs |