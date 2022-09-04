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
;PRTIME2
;Nov 12, 1984
;
    ORG 0xDAC0

BEGIN:	LXI H, MESSAGE ;set HL pointer to start of message data
	CALL DISPLAY	 ;display message
	LXI H, SECS1   ; SECS+1 = 0xF933+1 = 0xF934
	MOV A, M
	CALL LCD
	LXI H, SECS0	 ; SECS0 = 0xF933
	MOV A, M
	CALL LCD
	CALL CHGET
	JMP MENU
MESSAGE:	DB 0x54, 0x69, 0x6d, 0x65, 0x3a, 0x20
	DB 0x00 ;terminator

----->
;LCGamboa 8085 disassembler 2008

         ORG DAC0
LDAC0:   LXI H, 0xDADA
LDAC3:   CALL DISPLAY [0x5A58]
LDAC6:   LXI H, SECS1 [0xF934]
LDAC9:   MOV A, M 
LDACA:   CALL LCD [0x4B44]
LDACD:   LXI H, SECS0 [0xF933]
LDAD0:   MOV A, M 
LDAD1:   CALL LCD [0x4B44]
LDAD4:   CALL CHGET [0x12CB]
LDAD7:   JMP MENU [0x5797]
LDADA:   MOV D, H 
LDADB:   MOV L, C 
LDADC:   MOV L, L 
LDADD:   MOV H, L 
LDADE:   LDA 0x0020
```

```bash
% hexdump -C test0.co
00000000  21 da da cd 58 5a 21 34  f9 7e cd 44 4b 21 33 f9  |!???XZ!4?~?DK!3?|
00000010  7e cd 44 4b cd cb 12 c3  97 57 54 69 6d 65 3a 20  |~?DK??.?.WTime: |
00000020  00                                                |.               |
```

(DS seems to be borked, I'll check that out next...)