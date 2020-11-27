# 8602 Documentation
- [Introduction](#intro)
- [Specifications](#specs)
- [Documentation](#docs)

<a name="intro"></a>
## Introduction

<a name="specs"></a>
## Specifications
- 2.5 MHz clock rate
- Two display modes through an 800x600 SVGA signal
	- 400x256 pixel 2 color text mode
	- 200x128 pixel 16 color graphics mode
- 32 kB VRAM
	- Split into dual 16 kB buffers
- 16 kB SRAM with memory-mapped I/O
	- Up to 8 I/O addresses
	- Each I/O address can be used as either input or output
- Up to 8 MB of non-volatile flash memory
	- Split into 32 kB banks
	- Larger amounts of memory require additional glue logic
	- Up to 512 kB for a single flash chip with no additional glue logic

<a name="docs"></a>
## Documentation
- [Architecture](./arch.md)
	- [Registers](./arch.md#regs)
	- Flags
	- Memory Map
- Control Signals
	- Control Inputs
	- Control Outputs
- Assembly Language
	- Addressing Modes
	- Instruction Set
	- Instruction Matrix
