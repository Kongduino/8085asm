	ORG 0xDA00
BEGIN:		CALL CLS
			CALL HOME
			LXI H, VTIME
			CALL TIME
			LXI H, VDATE
			CALL DATE
			LXI H, VDAY
			CALL DAY
			LXI H, VDATE
			CALL DISPLAY
			CALL INITSRL
LOOP00:		MVI A,10 ; when a refresh of the menu is needed
			LXI H,CSRX
			MOV M,A
			LXI H,CSRY
			MVI A,2
			MOV M,A ; cursor 10,2
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
LOOP:		MVI A,11
			LXI H,CSRX
			MOV M,A
			LXI H,CSRY
			MVI A,4
			MOV M,A ; cursor 11,4
LOOP01:		CALL KYREAD ; Scans kbd for a key, returns in A, if any.
			JNZ HDLKBD
LOOP02:		CALL RCVX ; check rs232 queue
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
LOOP04:		CALL RV232C
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
			JP LOOP02
LOOP03:		LXI H,POSX
			MVI A,1
			MOV M,A
			JP LOOP ; we should handle the line. for now just loop back
HDLKBD:		CPI 81 ; Q
			JZ,THEEND
			CPI 113 ; q
			JZ,THEEND
			CPI 80 ; P
			JZ, PING
			CPI 112 ; P
			JZ, PING
			JP LOOP01
PING:		LXI H,CSRY
			MVI A,5
			MOV M,A ; cursor 1,6
			CALL ERAEOL
			LXI H,CSRY
			MVI A,7
			MOV M,A ; cursor 1,6
			CALL ERAEOL
			LXI H,CSRY
			MVI A,8
			MOV M,A ; cursor 1,6
			CALL ERAEOL
			LXI H,NPING
			CALL SNDSRL
			JP LOOP
THEEND:		CALL MENU

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
			JP SNDSRL0
RET00: RET
VDATE:	DS "yy/mm/dd ("
VDAY:	DS "ddd) "
VTIME:	DS "hh:mm:ss "
	DB 0
VGREET:	DS "LORA MESSENGER"
	DB 13,10
	DS "(P)ING (F)REQUENCY (B)W (S)F (Q)UIT"
	DB 0
VKEY:	DS "ENTER KEY:"
	DB 0
NPING: DS "AT+PSEND:50494E48"
		DB 10,0
SRLGREET:	DS "Hello"
	DB 13,10,0
POSX: DB 1