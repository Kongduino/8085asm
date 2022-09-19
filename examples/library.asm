	ORG 8000H
	CALL CLS
	CALL HOME
	MVI A, 0x80
	CALL HEX2ASC
	MVI A, 0x00
	CALL HEX2ASC
	LXI B, 0x0800
	CALL MULT5
	PUSH B
	MVI A, 7
	MVI B, 1
	CALL CURSOR
	POP B
	PUSH B
	MOV A, B	
	CALL HEX2ASC
	POP B
	MOV A, C
	CALL HEX2ASC
	MVI A, 1
	MVI B, 2
	CALL CURSOR
	MVI A, 0x00
	CALL HEX2ASC
	MVI A, 0x10
	CALL HEX2ASC
	MVI A, 7
	MVI B, 2
	CALL CURSOR
	LXI B, 0x0010
	CALL MULT5
	CALL MULT5 ; BC = BC * 25
	PUSH B
	MOV A, B
	CALL HEX2ASC
	POP B
	MOV A, C
	CALL HEX2ASC
	CALL CHGET
	RET

CURSOR:	; a = x. b = y
	PUSH H ; preserve HL
	LXI H, CSRX
	MOV M, A
	LXI H, CSRY
	MOV M, B ; cursor X, Y
	POP H
	RET

DIV8: ; BC = NUMBER
	MOV  A, B
	RAR
	MOV  B, A ; bit-shift right through carry
	MOV  A, C
	RAR
	MOV  C, A ; bit-shift right through carry
	MOV  A, B
	RAR
	MOV  B, A ; bit-shift right through carry
	MOV  A, C
	RAR
	MOV  C, A ; bit-shift right through carry
	MOV  A, B
	RAR
	MOV  B, A ; bit-shift right through carry
	MOV  A, C
	RAR
	MOV  C, A ; bit-shift right through carry

MULT5: ; BC = NUMBER
	PUSH H
	PUSH B
	POP H
	DAD B
	DAD B
	DAD B
	DAD B
	PUSH H
	POP B
	POP H
	RET

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
