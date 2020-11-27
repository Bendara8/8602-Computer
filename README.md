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

### Control Ouputs
