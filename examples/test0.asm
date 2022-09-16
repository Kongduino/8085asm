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
			MVI A,10
			LXI H,CSRX
			MOV M,A
			LXI H,CSRY
			MVI A,2
			MOV M,A ; cursor 10,2
			LXI H, VGREET
			CALL DISPLAY
			CALL INITSRL
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
			CALL CHGET
			CPI 81 ; Q
			JZ,THEEND
			CPI 113 ; q
			JZ,THEEND
			JP LOOP
			CPI 80 ; P
			JZ, PING
			CPI 112 ; P
			JZ, PING
			JP LOOP
PING:	MVI A,1
			LXI H,CSRX
			MOV M,A
			LXI H,CSRY
			MVI A,5
			MOV M,A ; cursor 1,5
			LXI H,NPING
			CALL DISPLAY
			JP LOOP
THEEND:		CALL MENU

INITSRL: ; display 5 setup bytes
			MVI H,9
			CALL BAUDST ; Set baud rate to 9, aka 19,200.
			MVI H,9
			MVI L, 1CH ; 0b11100 = 8N1
			CALL INZCOM ; init com
			MVI A,35
			LXI H,CSRX
			MOV M,A
			LXI H,CSRY
			MVI A,1
			MOV M,A ; cursor 35,1
			LXI H, STAT
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
			MVI A,1
			LXI H,CSRX
			MOV M,A
			LXI H,CSRY
			MVI A,6
			MOV M,A ; cursor 1,6
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
NPING: DS "PING!"
		DB 0
SRLGREET:	DS "Hello"
	DB 13,10,0