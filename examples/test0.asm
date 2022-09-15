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
			LXI H, VGREET
			CALL DISPLAY
			CALL INITSRL
LOOP:			CALL CHGET
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
			MOV M,A
			LXI H,NPING
			CALL DISPLAY
			JP LOOP
THEEND:		CALL MENU
			END

INITSRL: ; display 5 setup bytes
			MVI H,9
			CALL BAUDST ; Set baud rate to 9, aka 19,200.
			MVI A,1 ; baud, length, parity, stop bits, XON/XOFF
			LXI H,CSRX
			MOV M,A
			LXI H,CSRY
			MVI A,4
			MOV M,A
			LXI H, STAT
			MOV A,M
			CALL LCD
			INX H
			MOV A,M
			CALL LCD
			INX H
			MOV A,M
			CALL LCD
			INX H
			MOV A,M
			CALL LCD
			INX H
			MOV A,M
			CALL LCD
			CALL INZCOM ; init com
			LXI H,SRLGREET
			CALL SNDSRL

SNDSRL: MOV A,M
			CPI 0
			RZ
			CALL SD232C
			INX H
			JP SNDSRL
RET00: RET
VDATE:	DS "yy/mm/dd ("
VDAY:	DS "ddd) "
VTIME:	DS "hh:mm:ss "
	DB 13,10,0
VGREET:	DS "    LORA MESSENGER"
	DB 13,10
	DS "(P)ING      (F)REQUENCY (B)W        (S)F"
	DB 13,10
	DS "(Q)UIT"
	DB 13,10
EKEY:	DS "ENTER KEY:"
	DB 0
NPING: DS "PING!"
		DB 0
SRLGREET:	DS "Hello"
	db 13,10,0