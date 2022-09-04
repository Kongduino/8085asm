# 8085asm - Simple 8085/8080 assembler and disassembler

Simple 8085 assembler and disassembler for didatic use. `8085asm` generates the map file and intel hex file. It supports the EQU, ORG, DB, DS and END directives.

The 8085toZ80 application is a translator of 8085asm(8080) to z80asm.

## The Fork

I have forked this repository to improve and add to the assembler and disassembler. The first thing I did was to add the name of known (to me at least, there may be more) ROM functions and useful variables. There's a `header.asm` file with lots of `EQU`s, which will help you make your code more readable. Similarly, there's a `hex2label.h` include file for the disassembler, and the code has been updated to make use of the function labels:

![labels](img/labels.png)

I am planning to rework the code and make it – possibly – into a GUI app, backed by the 2 command-line applications. So... WIP.

In between, it seems to work:

![asm disasm](img/asm_dasm.png)

## UPDATES

**2022/09/04**
I added the known function and variable names to the compiler as labels, and they are recognized without the help of `EQU`s. A simple test code like this compiles as follows. I also added the creation of a plain binary file, with the `.co` extension.

```asm
	ORG 0xDA00
BEGIN:		LXI H, MESSAGE ;set HL pointer to start of message data
			CALL DISPLAY       ;display message
			LXI H, SECS1 ; SECS+1 = 0xF933+1 = 0xF934
			MOV A, M
			CALL LCD
			LXI H, SECS0
			MOV A, M
			CALL LCD
			CALL CHGET
			JMP MENU
MESSAGE:	DS "TIME = "
			DB 0
```
----->

```bash
> 8085asm test0.asm
LCGamboa 8085 assembler 2008

* Saving test0.map:
   Saving SYMBOLIC TABLE.
* RAM occupied:  (35 bytes):
* Saving test0.hex and test0.co:
 DA00H  21H
 DA01H  1AH
 DA02H  DAH
 DA03H  CDH
 DA04H  58H
 DA05H  5AH
 DA06H  21H
 DA07H  34H
 DA08H  F9H
 DA09H  7EH
 DA0AH  CDH
 DA0BH  44H
 DA0CH  4BH
 DA0DH  21H
 DA0EH  33H
 DA0FH  F9H
 DA10H  7EH
 DA11H  CDH
 DA12H  44H
 DA13H  4BH
 DA14H  CDH
 DA15H  CBH
 DA16H  12H
 DA17H  C3H
 DA18H  97H
 DA19H  57H
 DA1AH  C3H
 DA1BH  54H
 DA1CH  49H
 DA1DH  4DH
 DA1EH  45H
 DA1FH  20H
 DA20H  3DH
 DA21H  20H
 DA22H  00H
done!

> 8085dasm test0.hex
;LCGamboa 8085 disassembler 2008

         ORG DA00
LDA00:   LXI H, 0xDA1A
LDA03:   CALL DISPLAY [0x5A58]
LDA06:   LXI H, SECS1 [0xF934]
LDA09:   MOV A, M 
LDA0A:   CALL LCD [0x4B44]
LDA0D:   LXI H, SECS0 [0xF933]
LDA10:   MOV A, M 
LDA11:   CALL LCD [0x4B44]
LDA14:   CALL CHGET [0x12CB]
LDA17:   JMP MENU [0x5797]
LDA1A:   JMP 0x4954
LDA1D:   MOV C, L 
LDA1E:   MOV B, L 
LDA1F:   RIM 
LDA20:   DCR A 
LDA21:   RIM 
LDA22:   NOP 
2022-09-04 21:28:11 ~/Z80-8080-8085/TRS80/ASM/ > hexdump -C test0.co
00000000  21 1a da cd 58 5a 21 34  f9 7e cd 44 4b 21 33 f9  |!...XZ!4.~.DK!3.|
00000010  7e cd 44 4b cd cb 12 c3  97 57 c3 54 49 4d 45 20  |~.DK.....W.TIME |
00000020  3d 20 00                                          |= .             |
```

DS has been fixed. I need to add DW.