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
| __0__ | `lod A #(value)`<br>`sto P (address)` | `lod A (address)`<br>`lod P (address)` | `lod A B+#(offset)`<br>`lod P B+#(offset)` | `lod A B-#(offset)`<br>`lod P B-#(offset)` | `lod A P+#(index)`<br>`lod P P+#(index)` |  `lod A P+(address)`<br>`lod P P+(address)` |  `lod A P+B+#(offset)`<br>`lod P P+B+#(offset)` | `lod A P+B-#(offset)`<br>`lod P P+B-#(offset)` |