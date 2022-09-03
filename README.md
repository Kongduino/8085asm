# 8085asm - Simple 8085/8080 assembler and disassembler

Simple 8085 assembler and disassembler for didatic use. `8085asm` generates the map file and intel hex file. It supports the EQU, ORG, DB, DS and END directives.

The 8085toZ80 application is a translator of 8085asm(8080) to z80asm.

## The Fork

I have forked this repository to improve and add to the assembler and disassembler. The first thing I did was to add the name of known (to me at least, there may be more) ROM functions and useful variables. There's a `header.asm` file with lots of `EQU`s, which will help you make your code more readable. Similarly, there's a `hex2label.h` include file for the disassembler, and the code has been updated to make use of the function labels:

![labels](img/labels.png)

I am planning to rework the code and make it – possibly – into a GUI app, backed by the 2 command-line applications. So... WIP.

