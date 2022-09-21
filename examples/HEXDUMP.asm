	ORG 0xF4F7
LOOP:	CALL HOME
	CALL CLS
	LHLD CURRADD ; Start Address
	MVI A,1 ; display 8 rows
LOOP00:	PUSH PSW
	PUSH H
	LXI H, CSRY
	MOV M,A
	LXI H, CSRX
	MVI A,1
	MOV M,A ; set cursor to 1,row
	POP H
	MVI A,8 ; display 8 bytes
LOOP01:	PUSH PSW
	PUSH H
	MOV A,H ; display address 1st half
	CALL HEX2ASC ; Preserves HL
	MOV A,L ; display address 2nd half
	CALL HEX2ASC
	MVI A,0x3A ; Displays ':'
	CALL LCD
	MVI A,0x20 ; space
	CALL LCD
	MOV A,M
	CALL HEX2ASC ; display content of memory
	MVI A,0x20
	CALL LCD ; space
	POP H
	INX H ; increment address
	POP PSW
	SUI 1 ; byte counter
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
	MOV A,H
	CPI 31
	JP LOOP04 ; printable char?
	MVI A,0x2E ; or display '.'
LOOP04: CALL LCD
	POP PSW
	SUI 1 ; byte counter
	JNZ LOOP03
	POP PSW
	ADI 1 ; row counter
	CPI 8
	JNZ LOOP00
	SHLD CURRADD

LOOP02:	CALL CHGET ; this is where we decide what to do next
	CPI 81 ; Q
	JZ,THEEND
	CPI 113 ; q
	JZ,THEEND
	CPI 0x42 ; 'B' back
	JZ BACK00
	CPI 0x62 ; 'b' back
	JZ BACK00
	JZ,THEEND
	CPI 0x46 ; 'F' forward
	JZ FWD00
	CPI 0x66 ; 'f' forward
	JZ FWD00
	JMP LOOP02

BACK00: LXI H,CURRADD
	MOV A,M
	CPI 0
	JNZ BACK01
	INX H
	MOV A,M
	CP 0x80
	JZ LOOP02 ; if CURADD is 0x8000 then skip
BACK01:	LHLD CURRADD
	DCX H
	DCX H
	DCX H
	DCX H
	DCX H
	DCX H
	DCX H
	DCX H
	SHLD CURRADD ; Decrement address by 8
	JMP LOOP ; go back to the beginning and display the previous 8 bytes

FWD00: LXI H,CURRADD
	MOV A,M
	CPI 0
	JNZ FWD01
	INX H
	MOV A,M
	CP 0x80
	JZ LOOP02 ; if CURADD is 0x8000 then skip
FWD01: LHLD CURRADD
	INX H
	INX H
	INX H
	INX H
	INX H
	INX H
	INX H
	INX H
	SHLD CURRADD ; Increment address by 8
	JMP LOOP ; go back to the beginning and display the previous 8 bytes

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
