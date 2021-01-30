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
- [Simulator](./simulator.md)
	- [About](./simulator.md#about)
	- [Invocation](./simulator.md#invocation)
	- [Commands](./simulator.md#commands)
	- [Circuit Definition](./simulator.md#circuit-definition)
- [Emulator](./emulator.md)
	- [About](./emulator.md#about)
	- [Invocation](./emulator.md#invocation)

<a name="memory-map"></a>
## Memory Map
| Address Range | Description
| :-----------: | -----------
| $0000 - $7FFF | VRAM (32 kB)
| $8000 - $BEFF | General purpose SRAM (16 kB)
| $BF00 - $BFFF | I/O Page (256 B)
| $C000 - $FFFF | Current bank of flash memory (32 kB)

<a name="io-registers"></a>
## I/O Registers
| Address | Description
| :-----: | -----------
| $BF00   | Input <br> (keyboard input, video status, interrupt status, and LED status)
| $BF01   | Output register <br> (keyboard output, video control, interrupt clear, and LED control)
| $BF02   | Unused
| $BF03   | Unused
| $BF04   | Unused
| $BF05   | Unused
| $BF06   | Unused
| $BF07   | Unused

### Input
| Bit | Description
| :-: | -----------
| 0   | Clock interrupt status (active high)
| 1   | Keyboard interrupt status (active high)
| 2   | Status LED (active high)
| 3   | Unused
| 4   | Unused
| 5   | Unused
| 6   | Keyboard clock input
| 7   | Keyboard data input

### Output Register
| Bit | Description
| :-: | -----------
| 0   | Clock interrupt reset (active low)
| 1   | Keyboard interrupt reset (active low)
| 2   | Status LED (active high)
| 3   | Unused
| 4   | Unused
| 5   | Unused
| 6   | Keyboard clock output
| 7   | Keyboard data output
