# 8602 Emulator
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

<a name="about"></a>
## About
The 8602 emulator provides an environment for testing software. It runs much faster than the simulator, however it cannot be used to test hardware and merely replicates the behavior of fixed hardware in order to run software as close to realistic speeds as possible. Aside from a speed increase, the emulator has many more commands and features available that are useful for debugging software.

<a name="invocation"></a>
## Invocation
The emulator can be run with default options by simply invoking it through the terminal. The following options are available:

`-f, --flash <path>`
Override the default location of the file containing the flash memory data. Default location is `res/flash.bin`.

`-m, --microcode <path>`
Override the default location of the file containing the compiled microcode. Default location is `res/microcode.bin`.

`-r, --run`
Run the emulator immediately after invocation.