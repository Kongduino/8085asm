The assembler doesn't detect code errors (possible crashes).

Calling an  undefined label doesn't show errors!

The disassembler doesn't detect differences between code and data, so you need to nudge it:

```bash
8085dasm test0.hex 
;LCGamboa 8085 disassembler 2008
```

```asm
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

You can see that the start of the data is at `0xDADA`. So you pass that as an argument:

```bash
8085dasm test0.hex -d dada
;LCGamboa 8085 disassembler 2008
```

```asm
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
LDADA:   DB 0x54 [T]
LDADB:   DB 0x69 [i]
LDADC:   DB 0x6d [m]
LDADD:   DB 0x65 [e]
LDADE:   DB 0x3a [:]
LDADF:   DB 0x20 [ ]
LDAE0:   DB 0x00 [.]
```

This could be improved by passing intervals. But as of now it works.

