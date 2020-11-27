# Memory Documentation
- [Memory Map](#map)

<a name="map"></a>
## Memory Map
| Address Range | Description                          |
| :-----------: | -----------                          |
| $0000 - $3FFF | Current VRAM buffer (16 kB)          |
| $4000 - $7EFF | General purpose SRAM (16 kB)         |
| $7F00 - $7FFF | I/O Page (256 B)                     |
| $8000 - $FFFF | Current bank of flash memory (32 kB) |