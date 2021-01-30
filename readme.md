# 8602 Computer
Schematics, assembler, emulator, simulator, operating system, software, and documentation for the 8602 discrete logic computer

## About
The 8602 (_pronounced "eight-six-oh-two"_) is a minimalistic 8-bit computer built almost entirely of simple discrete logic chips from the 74 series logic family. Despite its simplicity, it features a VGA output and a PS/2 keyboard input, and can be easily expanded with additional I/O. The name 8602 is representative of its two primary inspirations: the Intel 8086 and the MOS 6502. The 8602 aims to be similar in capability and feature set to the 8086, while retaining the simplicity and overall feel of the 6502.

## Specifications
- 2.5 MHz clock rate
- Two display modes through an 800x600 SVGA signal
	- 400x256 pixel 8 color text mode
	- 200x128 pixel 64 color graphics mode
	- Both modes can be used at the same time
- 8-bit data bus
- 16-bit address bus
- 32 kB VRAM
- 16 kB SRAM with 256 bytes of memory-mapped I/O
	- Up to 8 I/O addresses
	- Each I/O address can be used as either input or output
- Up to 4 MB of non-volatile flash memory
	- Split into up to 16 chips with 16 16 kB banks each
	- 256 kB on a single flash chip with no additional glue logic
	- Larger amounts of memory require additional glue logic
- Microcoded CISC instruction set
- Full interrupt support, with ability to distinguish between interrupt sources
	- Clock interrupt occurs at regular intervals for timing purposes
	- Keyboard is interrupt driven

## Documentation
- [Architecture](./docs/arch.md)
	- [Diagram](./docs/arch.md#diagram)
	- [Registers](./docs/arch.md#registers)
	- [Flags](./docs/arch.md#flags)
- [System](./docs/system.md)
	- [Memory Map](./docs/system.md#memory-map)
	- [I/O Registers](./docs/system.md#io-registers)
- [Assembly Language](./docs/assembly.md)
	- [Addressing Modes](./docs/assembly.md#addressing-modes)
	- [Directives](./docs/assembly.md#directives)
	- [Instruction Set](./docs/assembly.md#instruction-set)
- [Control](./docs/control.md)
	- [Control Inputs](./docs/control.md#control-inputs)
	- [Control Outputs](./docs/control.md#control-outputs)
	- [Microcode](./docs/control.md#microcode)
- [Simulator](./docs/simulator.md)
	- [About](./docs/simulator.md#about)
	- [Invocation](./docs/simulator.md#invocation)
	- [Commands](./docs/simulator.md#commands)
	- [Circuit Definition](./docs/simulator.md#circuit-definition)
- [Emulator](./emulator.md)
	- [About](./emulator.md#about)
	- [Invocation](./emulator.md#invocation)

## Status
This project was started in September 2020. The overall design of the computer was finalized in December 2020. The schematics, simulator, and documentation are under progress. The assembler, emulator, operating system, and software are not started.
