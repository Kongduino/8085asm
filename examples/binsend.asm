	ORG 0xDA00
BEGIN:	CALL INITSRL
LOOP00:	CALL CLS ; when a refresh of the menu is needed
	MVI A, 10
	MVI B, 1
	CALL CURSOR
	LXI H, GREET0
	MVI A, 1
	MVI B, 2
	CALL CURSOR
	LXI H, GREET1
	CALL DISPLAY
	CALL CHGET
	MVI A, 1
	MVI B, 2
	CALL CURSOR
	LXI H, GREET2
	PUSH H
	CALL DISPLAY
	POP H
	CALL SNDSRL ; Sends ADDR
	MVI A, 32
	CALL LCD
LOOP01:	CALL RCVX
	JZ LOOP01
	LXI H, ADDR1
	MOV M, A
	CALL HEX2ASC
LOOP02:	CALL RCVX
	JZ LOOP02
	LXI H, ADDR0
	MOV M, A
	CALL HEX2ASC
	MVI A, 32
	CALL LCD
	LXI H, GREET3
	PUSH H
	CALL DISPLAY
	POP H
	CALL SNDSRL ; Sends ADDR
	MVI A, 32
	CALL LCD
LOOP03:	CALL RCVX
	JZ LOOP03
	LXI H, LENGTH1
	MOV M, A
	CALL HEX2ASC
LOOP04:	CALL RCVX
	JZ LOOP04
	LXI H, LENGTH0
	MOV M, A
	CALL HEX2ASC
	CALL CHGET
	RET


INITSRL:	CALL CLSCOM
	MVI H,9
	MVI L, 1CH ; 0b11100 = 8N1
	SETC ; FOR RS232 <--- Very important!
	CALL INZCOM ; init com
	MVI A,35
	LXI H,CSRX
	MOV M,A
	LXI H,CSRY
	MVI A,1
	MOV M,A ; cursor 35,1
	LXI H, STAT ; display 5 setup bytes
	MOV A,M
	CALL LCD ; baud
	INX H
	MOV A,M
	CALL LCD ; length
	INX H
	MOV A,M
	CALL LCD ; parity
	INX H
	MOV A,M
	CALL LCD ; stop bits
	INX H
	MOV A,M
	CALL LCD ; XON/XOFF
	LXI H,SRLGREET
	CALL SNDSRL
	RET

SNDSRL:	PUSH H
	IN 0xC8 ; before you use the UART, it is good practice to clear the UART receiver buffer
	; register with an input from port C8. This is done, for example, at 6CE5.
	MVI A,1
	LXI H,CSRX
	MOV M,A
	LXI H,CSRY
	MVI A,6
	MOV M,A ; cursor 1,6
	CALL ERAEOL
	POP H
SNDSRL0: 	MOV A,M
	CPI 0
	RZ
	CALL SD232C
	MOV A,M
	CALL LCD ; output the string also to LCD
	INX H
	JMP SNDSRL0

CURSOR:	; a = x. b = y
	PUSH H ; preserve HL
	LXI H, CSRX
	MOV M, A
	LXI H, CSRY
	MOV M, B ; cursor X, Y
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
	ADI 0x30 ; Add 0x30 for "0" to "9"
	CPI 0x3A
	JM HEX2ASC0
	ADI 7 ; add 7 extra for "A" to "F"
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

GREET0: DS "Binary Download Transfer"
	DB 0
GREET1: DS "Hit a key when your computer is ready to transfer"
	DB 0
GREET2: DS "ADDR"
	DB 0
GREET3: DS "LENGTH"
	DB 0
ADDR0: DB 0
ADDR1: DB 0
LENGTH0: DB 0
LENGTH1: DB 0



