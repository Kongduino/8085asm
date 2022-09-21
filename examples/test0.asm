	ORG 0xF3CE
BEGIN:	CALL INITSRL
LOOP00:	CALL CLS ; when a refresh of the menu is needed
			CALL HOME
			MVI A,10
			LXI H,CSRX
			MOV M,A
			LXI H,CSRY
			MVI A,1
			MOV M,A ; cursor 10,1
			LXI H, VGREET
			CALL DISPLAY
			MVI A,1
			LXI H,CSRX
			MOV M,A
			LXI H,CSRY
			MVI A,4
			MOV M,A ; cursor 1,4
			LXI H,VKEY
			CALL DISPLAY
LOOP:	MVI A,11
			LXI H,CSRX
			MOV M,A
			LXI H,CSRY
			MVI A,4
			MOV M,A ; cursor 11,4
LOOP01:	CALL KYREAD ; Scans kbd for a key, returns in A, if any.
			JNZ HDLKBD
LOOP02:	CALL RCVX ; check rs232 queue
			JZ LOOP01
			LXI H,CSRY
			MVI A,8
			MOV M,A
			LXI H,POSX
			MOV A,M
			LXI H,CSRX
			MOV M,A ; cursor X,8
			CPI 1
			JNZ LOOP04 ; if X = 1 we're on a new line, erase lines 6 & 8
			LXI H,CSRY
			MVI A,6
			MOV M,A
			CALL ERAEOL
			LXI H,CSRY
			MVI A,8
			MOV M,A
			CALL ERAEOL
LOOP04:	CALL RV232C
			CPI 10
			JZ LOOP03 ; we have a line if it's lf
			CPI 13
			JZ LOOP02 ; skip if it's cr
			CALL LCD
			LXI H,POSX
			MOV A,M
			INR A
			MOV M,A
			CPI 40
			JNZ LOOP02
			MVI A,1
			MOV M,A
			JMP LOOP02
LOOP03:	LXI H,POSX
			MVI A,1
			MOV M,A
			JMP LOOP ; we should handle the line. for now just loop back
HDLKBD:	CPI 81 ; Q
			JZ,THEEND
			CPI 113 ; q
			JZ,THEEND
			CPI 80 ; P
			JZ, PING
			CPI 112 ; P
			JZ, PING
			JMP LOOP01
PING:	LXI H,CSRY
			MVI A,5
			MOV M,A
			CALL ERAEOL
			LXI H,CSRY
			MVI A,7
			MOV M,A ; cursor 7,6
			CALL ERAEOL
			LXI H,CSRY
			MVI A,8
			MOV M,A ; cursor 1,8
			CALL ERAEOL
			LXI H,NPING
			CALL SNDSRL
			JMP LOOP

THEEND:	CALL CLSCOM
			JMP MENU

DOFREQ:	CALL CLS
			CALL HOME
			LXI H, FQMENU
			CALL DISPLAY
			CALL CHGET
			CPI 0x42 ; B
			JZ,LOOP00
			CPI 0x62 ; q
			JZ,LOOP00
			CPI 0x30 ; 0
			JZ DOFREQ0
			CPI 0x31 ; 1
			JZ DOFREQ1
			CPI 0x32 ; 2
			JZ DOFREQ2
			CPI 0x33 ; 3
			JZ DOFREQ3
			JMP DOFREQ

DOFREQ0:	LXI H,FCHOICE0
			CALL SNDSRL
			JMP DOFREQ
DOFREQ1:	LXI H,FCHOICE1
			CALL SNDSRL
			JMP DOFREQ
DOFREQ2:	LXI H,FCHOICE2
			CALL SNDSRL
			JMP DOFREQ
DOFREQ3:	LXI H,FCHOICE3
			CALL SNDSRL
			JMP DOFREQ

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

SNDSRL: 	PUSH H
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

VGREET:	DS "LORA MESSENGER"
	DB 13,10
	DS "(P)ING (F)REQUENCY (B)W (S)F (Q)UIT"
	DB 0
VKEY:	DS "ENTER KEY:"
	DB 0
NPING: DS "AT+PSEND:50494E48"
		DB 10,0
SRLGREET:	DS "Hello"
	DB 0

FQMENU0:	"Enter freq: "
		DB 0
FQMENU:	DS "(0)868.000 (1)868.125 (2)868.250"
		DB 13,10
		DS "(3)915.000 (B)ACK"
		DB 0
FCHOICE0: "AT+PFREQ=868000000"
		DB 13, 10, 0
FCHOICE0: "AT+PFREQ=868125000"
		DB 13, 10, 0
FCHOICE0: "AT+PFREQ=868250000"
		DB 13, 10, 0
FCHOICE0: "AT+PFREQ=915000000"
		DB 13, 10, 0
POSX: DB 1
