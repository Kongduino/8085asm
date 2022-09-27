	ORG 0xF400 ; run 8085asm once first to know the correct ORG
LOOP:	CALL CLS
	MVI A,1 ; display 8 rows
LOOP00:	PUSH PSW
	LXI H, CSRY
	MOV M,A
	LXI H, CSRX
	MVI A,1
	MOV M,A
	LHLD CURRADD ; Start Address
	SHLD LASTADD ; Save that address for later
	MOV M,A ; set cursor to 1,row
	MOV A,H ; display address 1st half
	CALL HEX2ASC ; Preserves HL
	MOV A,L ; display address 2nd half
	CALL HEX2ASC
	MVI A,0x3A ; Displays ':'
	CALL LCD
	MVI A,0x20 ; space
	CALL LCD
	LHLD CURRADD ; Start Address
	MVI A,8 ; display 8 bytes
LOOP01:	PUSH PSW
	PUSH H
	MOV A,M
	CALL HEX2ASC ; display content of memory
	MVI A,0x20
	CALL LCD ; space
	POP H
	INX H ; increment address
	POP PSW
	DCR A ; byte counter
	JNZ LOOP01
	DCX H
	DCX H
	DCX H
	DCX H
	DCX H
	DCX H
	DCX H
	DCX H
	MVI A,8 ; display 8 bytes ASCII or .
LOOP03:	PUSH PSW
	PUSH H
	MOV A,M
	CPI 31
	JP LOOP04 ; printable char?
	MVI A,0x2E ; or display '.'
LOOP04: CALL LCD
	POP H
	INX H
	POP PSW
	DCR A ; byte counter
	JNZ LOOP03
	SHLD CURRADD
	POP PSW
	ADI 1 ; row counter
	CPI 8
	JNZ LOOP00

LOOP02:	CALL CHGET ; this is where we decide what to do next
	CPI 81 ; Q
	JZ,THEEND
	CPI 113 ; q
	JZ,THEEND
	CPI 0x1e ; 'arrow up' back
	JZ BACK00
	CPI 0x20 ; ' ' forward
	JZ LOOP
	CPI 0x1F ; 'arrow down' forward
	JZ LOOP
	CPI 0x2F ; '/' search
	JZ LOOKUP
	JMP LOOP02

BACK00:	LHLD LASTADD
	LXI B, 0xFFC8 ; -72
	DAD B
	SHLD CURRADD ; Decrement address by 72
	JMP LOOP ; go back to the beginning and display the previous 8 bytes

LOOKUP:	MVI A,1
	LXI H, CSRX
	MOV M,A
	LXI H, CSRY
	MVI A,4
	MOV M,A
	CALL ERAEOL
	MVI A,5
	LXI H, CSRX
	MOV M,A
	LXI H, ENTERSEARCH
	CALL DISPLAY
	CALL INLIN
	CALL CHGET
	JMP BACK00

THEEND: RET

HEX2ASC: ; IN: A = BYTE
	PUSH H ; preserve HL
	PUSH PSW ; preserve A for now
	RRC
	RRC
	RRC
	RRC ; A >> 4
	ANI 0x0F ; & 0b00001111
	ADI 0x30 ; Add 0x30 for '0' to '9'
	CPI 0x3A
	JM HEX2ASC0
	ADI 7 ; add 7 extra for 'A' to 'F'
HEX2ASC0:	CALL LCD ; display first char
	POP PSW ; retrieve A
	ANI 0x0F ; same thing on lower nibble
	ADI 0x30
	CPI 0x3A
	JM HEX2ASC1
	ADI 7
HEX2ASC1:	CALL LCD
	POP H ; retrieve HL
	RET

CURRADD: DB 0x00, 0x80 ; 0x8000
LASTADD: DB 0x00, 0x80 ; 0x8000

ENTERSEARCH: DS "Enter Hex Line: "
	DB 0