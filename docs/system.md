# 8602 System
- [Architecture](./arch.md)
	- [Diagram](./arch.md#diagram)
	- [Registers](./arch.md#registers)
	- [Flags](./arch.md#flags)
- [System](./system.md)
	- [Memory Map](./system.md#memory-map)
	- [I/O Registers](./system.md#io-registers)
- [Assembly Language](./assembly.md)
	- [Addressing Modes](./assembly.md#addressing-modes)
	- [Directives](./assembly.md#directives)
	- [Instruction Set](./assembly.md#instruction-set)
- [Control](./control.md)
	- [Control Inputs](./control.md#control-inputs)
	- [Control Outputs](./control.md#control-outputs)
	- [Microcode](./control.md#microcode)

<a name="memory-map"></a>
## Memory Map
| Address Range | Description                          |
| :-----------: | -----------                          |
| $0000 - $3FFF | Current VRAM buffer (16 kB)          |
| $4000 - $7EFF | General purpose SRAM (16 kB)         |
| $7F00 - $7FFF | I/O Page (256 B)                     |
| $8000 - $FFFF | Current bank of flash memory (32 kB) |

<a name="io-registers"></a>
## I/O Registers
| Address | Description
| :-----: | -----------
| $7F00   | Output register <br> (keyboard output, video control, interrupt clear, and LED control) |
| $7F01   | Input <br> (keyboard input, video status, interrupt status, and LED status)             |
| $7F02   | Color palette memory address register                                                   |
| $7F03   | Color palette memory data register                                                      |

### Output Register
| Bit | Description
| :-: | -----------
| 0   | Clock interrupt reset (active low)
| 1   | Keyboard interrupt reset (active low)
| 2   | Status LED (active high)
| 3   | Video Mode (0: Text Mode, 1: Graphics Mode)
| 4   | Select VRAM buffer to display
| 5   | Select VRAM buffer to access
| 6   | Keyboard clock output
| 7   | Keyboard data output

### Input
| Bit | Description
| :-: | -----------
| 0   | Clock interrupt status (active high)
| 1   | Keyboard interrupt status (active high)
| 2   | Status LED (active high)
| 3   | Video Mode (0: Text Mode, 1: Graphics Mode)
| 4   | Currently displayed VRAM buffer
| 5   | Currently accessed VRAM buffer
| 6   | Keyboard clock input
| 7   | Keyboard data input