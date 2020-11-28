# 8602 Assembly Language
- [Addressing Modes](./assembly.md#modes)
- [Instruction Set](./assembly.md#set)
- [Instruction Matrix](./assembly.md#matrix)

<a name="modes"></a>
## Addressing Modes
| Mode            | Syntax                               | Description
| :--:            | ------                               | -----------
| Register        | `(reg)`                              | Inside register
| Immediate       | `#(value)`                           | Immediately after instruction
| Absolute        | `(address)`                          | At an address
| Indirect        | `@(address)`                         | At a pointer at an address
| Relative        | `I+#(offset)` <br> `I-#(offset)`     | At an offset to the instruction pointer
| Offset          | `B+#(offset)` <br> `B-#(offset)`     | At an offset to the base pointer
| Immediate Index | `P+#(index)`                         | At the pointer with an immediate index 
| Absolute Index  | `P+(address)`                        | At the pointer with an index at the address 
| Offset Index    | `P+B+#(offset)` <br> `P+B-#(offset)` | At the pointer with an index at an offset to the base pointer 

<a name="set"></a>
## Instruction Set
### Data Movement
| Mnemonic | Description
| :------: | -----------

### Arithmetic and Logic
| Mnemonic | Description
| :------: | -----------

### Control Flow
| Mnemonic | Description
| :------: | -----------

<a name="matrix"></a>
## Instruction Matrix
| Hex   | __0<br>1__ | __0<br>1__ | __0<br>1__ | __0<br>1__ | __0<br>1__ | __0<br>1__ | __0<br>1__ | __0<br>1__ |
| :---: | :--------: | :--------: | :--------: | :--------: | :--------: | :--------: | :--------: | :--------: |
| __0__ | `lod A #`<br>`sto P _` | `lod A _`<br>`lod P _` | `lod A B+#`<br>`lod P B+#` | `lod A B-#`<br>`lod P B-#` | `lod A P+#`<br>`lod P P+#` |  `lod A P+_`<br>`lod P P+_` |  `lod A P+B+#`<br>`lod P P+B+#` | `lod A P+B-#`<br>`lod P P+B-#` |