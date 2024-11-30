# MPU6050

# LCD


# PCF8674x
## Datasheet reference from 
- NXP Semiconductors: [PCF8764 Datasheet](https://www.nxp.com/docs/en/data-sheet/PCF8574_PCF8574A.pdf)
- Texas Instruments: [PCF8764A](https://www.ti.com/lit/ds/symlink/pcf8574a.pdf)
## Register map
3 modify pin: A0, A1, A2. Value from (000 -> 111)
### With PCF8674
7-bit hexadecimal address without R/nW: 0x20h to 0x27h  
For example with R/nW: [(0x27h << 1) | 0xF]/ [0x27h << 1] 
### With PCF8674A
7-bit hexadecimal address without R/nW: 0x38h to 0x3Fh  
For example with R/nW: [(0x3Fh << 1) | 0xF]/ [0x3Fh << 1] 
## Message format
```
<S> (addr)<A6-A5-A4-A3-A2-A1-A0-R/W> <ACK> (data)[<P7-P6-P5-P4-P3-P2-P1-P0> <ACK>...]<P>
```
## Connect to LCD 1602
Data: 0x28 (0010 1000)  
--->: u:0010; l:1000  
--->: 0x2C 0x28 0x8C 0x88


<style>
table {
  border-collapse: collapse;
  width: 100%;
}
th, td {
  border: 1px solid black;
  text-align: center;
  padding: 8px;
}
</style>


| PCF8674A | LCD1602 |   Bit   |
|:--------:|:-------:|:-------:|
|    P7    |    D7   |    0    |
|    P6    |    D6   |    0    |
|    P5    |    D5   |    1    |
|    P4    |    D4   |    0    |
|    P3    |    1    |    1    |
|    P2    |    E    |    1    |
|    P1    |    RW   |    0    |
|    P0    |    RS   |    0    |


