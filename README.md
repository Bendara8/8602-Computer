# 8602-Computer
Tools, Simulator, CAD, and Documentation for the 8602 Discreet Logic Computer

## About
The 8602 is a minimalistic computer that is composed entirely of simple discreet logic chips.

## 8602 Spec
### Control Outputs
00. Data Bus Assert Select
	- AO
	- FO
	- MO
	- KO
	- PL/PH
	- BL/BH
	- IL/IH
	- HO
03. HB High Byte
04. Data Bus Load Select
	- AI Accumulator
	- MI Memory
	- KI Bank
	- XI Index
	- CI Instruction Code
	- TL Transfer Low Byte
	- TH Transfer High Byte
	- --
07. FI Flag In
08. Address Bus Assert
	- PA Pointer
	- BA Base Pointer
	- SA Stack Pointer
	- IA Instruction Pointer
10. TA Transfer Pointer
11. Address Bus Load Select
	- PI Pointer
	- BI Base Pointer
	- SI Stack Pointer
	- II Instruction Pointer
13. -- Address Bus Load Disable
14. FB Load Flags from Data Bus
15. KZ Bank Zero Output
16. XZ Index Zero Output
17. XN Index Logical Not Output
18. XC Index Carry In
19. EZ ALU Zero Accumulator Input
20. EN ALU Logical Not Data Bus Input
21. EC ALU Carry In
22. ALU Operation Select
	- ES Sum
	- ER Right Shift Data Bus Input
	- EA Logical And
	- EO Logical Or