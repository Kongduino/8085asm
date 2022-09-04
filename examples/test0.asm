	ORG 0xDA00
BEGIN:		LXI H, MESSAGE ;set HL pointer to start of message data
			CALL DISPLAY       ;display message
			LXI H, SECS1 ; SECS+1 = 0xF933+1 = 0xF934
			MOV A, M
			CALL LCD
			LXI H, SECS0
			MOV A, M
			CALL LCD
			CALL CHGET
			JMP MENU
MESSAGE:	DS "TIME = "
			DB 0