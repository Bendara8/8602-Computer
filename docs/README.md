# 8602 Documentation
- [About](#about)
- [Specifications](#specs)
- [Documentation](#docs)

<a name="about"></a>
## About
The 8602 (_pronounced "eight-six-oh-two"_) is a minimalistic 8-bit computer built almost entirely of simple discreet logic chips from the 74HC logic family. Despite its simplicity, it features a VGA output and a PS/2 keyboard input, and can be easily expanded with additional I/O. The name 8602 is representative of its two primary inspirations: the Intel 8086 and the MOS 6502. The 8602 aims to be similar in capability and feature set to the 8086, while retaining the simplicity and overall feel of the 6502.

<a name="specs"></a>
## Specifications
- 2.5 MHz clock rate
- Two display modes through an 800x600 SVGA signal
	- 400x256 pixel 2 color text mode
	- 200x128 pixel 16 color graphics mode
	- Both modes can be displayed at the same time by precisely switching between the two
- 8-bit data bus
- 16-bit address bus
- 32 kB VRAM
	- Split into dual 16 kB buffers
- 16 kB SRAM with 256 bytes of memory-mapped I/O
	- Up to 8 I/O addresses
	- Each I/O address can be used as either input or output
- Up to 8 MB of non-volatile flash memory
	- Split into 32 kB banks
	- Larger amounts of memory require additional glue logic
	- Up to 512 kB for a single flash chip with no additional glue logic
- Microcoded CISC instruction set
- Full interrupt support, with ability to distinguish between interrupt sources
	- Clock interrupt occurs at regular intervals for timing purposes
	- Keyboard is interrupt driven

<a name="docs"></a>
## Documentation
- [Architecture](./arch.md)
	- [Registers](./arch.md#regs)
	- [Flags](./arch.md#flags)
- [Memory](./memory.md)
	- [Memory Map](./memory.md#map)
	- [I/O Registers](./memory.md#io)
- [Assembly Language](./assembly.md)
	- [Addressing Modes](./assembly.md#modes)
	- [Instruction Set](./assembly.md#set)
	- [Instruction Matrix](./assembly.md#matrix)
- [Control](./control.md)
	- [Control Inputs](./control.md#inputs)
	- [Control Outputs](./control.md#outputs)
