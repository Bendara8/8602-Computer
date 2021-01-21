# 8602 Simulator
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
The 8602 simulator is a very detailed simulation of the computer. It simulates each chip in the system on a nanosecond by nanosecond timescale. Its purpose is to validate hardware before actually building the computer. The simulator can also be used to easily test hardware changes to the 8602 without having to actually change the physical hardware. It is not recommended to develop software for the 8602 using the simulator due to low simulation speed and a lack of software oriented commands. Instead, the emulator should be used for software development.

<a name="invocation"></a>
## Invocation
The simulator must be invoked with a filepath to a text file containing the circuit definition. By convention, the `.src` suffix is used for these circuit definition files. The circuit definition syntax is explained in the [Circuit Definition](#circuit-definition) section.

`simulator circuit-def.src`

<a name="commands"></a>
## Commands
Here is a list of the available commands with links to more information on the usage of each command.

### Navigation
| Command           | Description
| :-----:           | -----------
| [`nano`](#nano)   | Runs the simulation for a given number of nanoseconds
| [`run`](#run)     | Runs the simulation until the user interrupts the simulation with `Ctrl+C`
| [`quit`](#quit)   | Quit the simulation

### Information
| Command               | Description
| :-----:               | -----------
| [`updates`](#updates) | See all scheduled updates
| [`chip`](#chip)       | See the status of a chip
| [`bus`](#bus)         | See the status of a bus
| [`net`](#net)         | See the status of a net

### Memory
| Command       | Description
| :-----:       | -----------
| [`hex`](#hex) | Hexdump a portion of the memory contained in a chip

### Manipulation
| Command           | Description
| :-----:           | -----------
| [`set`](#set)     | Set a bus or net to a value

<a name="circuit-definition"></a>
## Circuit Definition
Circuit definitions contain three definition sections: one for buses, one for nets, and one for chips. A net is essentially a wire that connects any number of chips together through their pins. A bus is collection of nets that form a single value. The sections can be split up and rearranged in any way, provided that buses and nets referenced in chip definition sections are defined prior to that section. Comment lines begin with a hash symbol `#` and are ignored. Comments can occur on any line, provided that they are the last thing on that line. Below is an example circuit definition for an 8-bit adder/subtractor.
```
# Example 8-bit Adder/Subtractor

bus {
	IN_A 8
	IN_B 8
	NEG_B 8
	OUT 8
}

net {
	SUB = 1
	CARRY_OUT
	CARRY_INTERNAL
}

chip {
	_74HC86 {
		in:0,2,4,6 = IN_B:0-3
		in:1,3,5,7 = SUB
		out:0-3 = NEG_B:0-3
	}
	_74HC86 {
		in:0,2,4,6 = IN_B:4-7
		in:1,3,5,7 = SUB
		out:0-3 = NEG_B:4-7
	}
	_74HC283 ADDER_LOW {
		in:0-3 = IN_A:0-3
		in:4-7 = NEG_B:0-3
		in:8 = SUB
		out:0-3 = OUT:0-3
		out:4 = CARRY_INTERNAL
	}
	_74HC283 ADDER_HIGH {
		in:0-3 = IN_A:4-7
		in:4-7 = NEG_B:4-7
		in:8 = CARRY_INTERNAL
		out:0-3 = OUT:4-7
		out:4 = CARRY_OUT
	}
}
```
Each line in the `bus` section has a name for the bus, as well as the width of the bus. For example, the `IN_B` bus is 8 bits wide. Each line in the `net` section has a name for the net, and an optional inital value for the net. Inital values can be either 0 or 1, and all nets are initalized to 0 by default if no intial value is given. For example, the `SUB` net is intialized to 1, while the `CARRY_OUT` net is left with default initialization of 0. All buses are always initalized to 0.

The `chip` section is the most complex; it defines what chip to use, an optional chip name, and how each pin of each chip should be connected. The first entry in the chip section begins with `_74HC86` signifying that this chip is a 74HC86 Quad 2-input XOR Gate. An underscore must be included before chip types beginning with numbers to prevent the chip type from being interpreted as a number. On the next line, input pins 0, 2, 4, and 6 are connected to nets 0 through 3 of the `IN_B` bus. Specifically, input pin 0 is connected to net 0 of `IN_B`, input pin 2 is connected to net 1 of `IN_B`, and so on. The number of input pins being connected must match the number of nets specified, unless there is only a single net specified in which case all inputs are connected to that net. There is an example of this on the next line: input pins 1, 3, 5, and 7 are all connected to the `SUB` net.

Notice that buses must be followed by a colon specifying which nets of that bus to connect, while nets are not followed by colon. Chips can also be given a name; the two 74HC283 4-Bit Adders are given the names `ADDER_LOW` and `ADDER_HIGH` so that they can be referenced during the simulation. Each chip available to the simulation models a real integrated circuit, although the pin numbers are changed to make the connection process easier. A full list of chips can be found in the following table, which links to pin information for each chip.

| Chip                | Description
| :--:                | -----------
| [CLK40M](#CLK40M)   | 40 MHz Clock
| [74HC00](#74HC00)   | Quad 2-input NAND Gate
| [74HC30](#74HC30)   | 8-input NAND Gate
| [74HC02](#74HC02)   | Quad 2-input NOR Gate
| [74HC04](#74HC04)   | Hex NOT Gate
| [74HC08](#74HC08)   | Quad 2-input AND Gate
| [74HC21](#74HC21)   | Dual 4-input AND Gate
| [74HC32](#74HC32)   | Quad 2-input OR Gate
| [74HC86](#74HC86)   | Quad 2-input XOR Gate
| [74AC157](#74HC157) | 4-bit 2-input Multiplexer
| [74AC153](#74HC153) | 2-bit 4-input Multiplexer
| [74AC161](#74AC161) | 4-bit Synch Counter Asynch Reset
| [74HC283](#74HC283) | 4-bit Adder
| [74HC377](#74HC377) | 8-bit Register
| [74HC541](#74HC541) | 8-bit Bus Driver

## Command Details
<a name="nano"></a>
### `nano`

<a name="clock"></a>
### `clock`

<a name="run"></a>
### `run`

<a name="quit"></a>
### `quit`

<a name="updates"></a>
### `updates`

<a name="chip"></a>
### `chip`

<a name="bus"></a>
### `bus`

<a name="net"></a>
### `net`

<a name="hex"></a>
### `hex`

<a name="set"></a>
### `set`

## Chip Details
<a name="CLK40M"></a>
### CLK40M 40 MHz Clock
| Input | Description | Output | Description
| :---: | ----------- | :----: | -----------
| 0     | Enable      | 0      | Clock

<a name="74HC00"></a>
### 74HC00 Quad 2-input NAND Gate
| Input | Description | Output | Description
| :---: | ----------- | :----: | -----------
| 0 - 1 | Gate A      | 0      | Gate A
| 2 - 3 | Gate B      | 1      | Gate B
| 4 - 5 | Gate C      | 2      | Gate C
| 6 - 7 | Gate D      | 3      | Gate D

<a name="74HC30"></a>
### 74HC30 8-input NAND Gate
| Input | Description | Output | Description
| :---: | ----------- | :----: | -----------
| 0 - 7 | Gate        | 0      | Gate

<a name="74HC02"></a>
### 74HC02 Quad 2-input NOR Gate
| Input | Description | Output | Description
| :---: | ----------- | :----: | -----------
| 0 - 1 | Gate A      | 0      | Gate A
| 2 - 3 | Gate B      | 1      | Gate B
| 4 - 5 | Gate C      | 2      | Gate C
| 6 - 7 | Gate D      | 3      | Gate D

<a name="74HC04"></a>
### 74HC04 Hex NOT Gate
| Input | Description | Output | Description
| :---: | ----------- | :----: | -----------
| 0     | Gate A      | 0      | Gate A
| 1     | Gate B      | 1      | Gate B
| 2     | Gate C      | 2      | Gate C
| 3     | Gate D      | 3      | Gate D
| 4     | Gate E      | 4      | Gate E
| 5     | Gate F      | 5      | Gate F

<a name="74HC08"></a>
### 74HC08 Quad 2-input AND Gate
| Input | Description | Output | Description
| :---: | ----------- | :----: | -----------
| 0 - 1 | Gate A      | 0      | Gate A
| 2 - 3 | Gate B      | 1      | Gate B
| 4 - 5 | Gate C      | 2      | Gate C
| 6 - 7 | Gate D      | 3      | Gate D

<a name="74HC21"></a>
### 74HC21 Dual 4-input AND Gate
| Input | Description | Output | Description
| :---: | ----------- | :----: | -----------
| 0 - 3 | Gate A      | 0      | Gate A
| 4 - 7 | Gate B      | 1      | Gate B

<a name="74HC32"></a>
### 74HC32 Quad 2-input OR Gate
| Input | Description | Output | Description
| :---: | ----------- | :----: | -----------
| 0 - 1 | Gate A      | 0      | Gate A
| 2 - 3 | Gate B      | 1      | Gate B
| 4 - 5 | Gate C      | 2      | Gate C
| 6 - 7 | Gate D      | 3      | Gate D

<a name="74HC86"></a>
### 74HC86 Quad 2-input XOR Gate
| Input | Description | Output | Description
| :---: | ----------- | :----: | -----------
| 0 - 1 | Gate A      | 0      | Gate A
| 2 - 3 | Gate B      | 1      | Gate B
| 4 - 5 | Gate C      | 2      | Gate C
| 6 - 7 | Gate D      | 3      | Gate D

<a name="74AC157"></a>
### 74AC157 4-bit 2-input Multiplexer
| Input | Description | Output | Description
| :---: | ----------- | :----: | -----------
| 0 - 3 | Data 0      | 0 - 3  | Selected Data
| 4 - 7 | Data 1      |        |
| 8     | Select      |        |
| 9     | ~Enable     |        |

<a name="74AC153"></a>
### 74AC153 2-bit 4-input Multiplexer
| Input   | Description | Output | Description
| :---:   | ----------- | :----: | -----------
| 0 - 1   | Data 0      | 0 - 1  | Selected Data
| 2 - 3   | Data 1      |        |
| 4 - 5   | Data 2      |        |
| 6 - 7   | Data 3      |        |
| 8 - 9   | Select      |        |
| 10 - 11 | ~Enables    |        |

<a name="74AC161"></a>
### 74AC161 4-bit Synch Counter Asynch Reset
| Input | Description | Output | Description
| :---: | ----------- | :----: | -----------
| 0 - 3 | Data        | 0 - 3  | Latched Data
| 4     | ~Load       | 4      | Ripple
| 5     | Count       |        |
| 6     | Ripple      |        |
| 7     | ~Clear      |        |
| 8     | Clock       |        |

<a name="74HC283"></a>
### 74HC283 4-bit Adder
| Input | Description | Output | Description
| :---: | ----------- | :----: | -----------
| 0 - 3 | Data A      | 0 - 3  | Sum of A and B
| 4 - 7 | Data B      | 4      | Carry out
| 8     | Carry in    |        |

<a name="74HC377"></a>
### 74HC377 8-bit Register
| Input | Description | Output | Description
| :---: | ----------- | :----: | -----------
| 0 - 7 | Data        | 0 - 7  | Latched Data
| 8     | ~Load       |        |
| 9     | Clock       |        |

<a name="74HC541"></a>
### 74HC541 8-bit Bus Driver
| Input | Description | Output | Description
| :---: | ----------- | :----: | -----------
| 0 - 7 | Data        | 0 - 7  | Bus Connection
| 8 - 9 | ~Enables    |        |