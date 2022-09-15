	ORG 0xDA00
BEGIN:	CALL HOME
			CALL CLS
			LXI H, MESSAGE ;set HL pointer to start of message data
			CALL DISPLAY       ;display message
			LXI H, SECS1 ; SECS+1 = 0xF933+1 = 0xF934
			MOV A, M
			CALL LCD
			LXI H, SECS0
			MOV A, M
			CALL LCD
			CALL CHGET
			CALL MENU
			END
MESSAGE:	DS "TIME = "
			DB 0