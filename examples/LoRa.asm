	ORG 0xEE0A
BEGIN:	CALL INITSRL
LOOP00:	CALL CLS ; when a refresh of the menu is needed
	CALL SETSYS ; Sets system line (locks line 8).
	MVI A,10
	MVI B,1
	CALL CURSOR
	LXI H, VGREET
	CALL DISPLAY
	MVI A,1
	MVI B,8
	CALL CURSOR
	LXI H, MAINFNMENU
	CALL DISPLAY
	MVI A,1
	MVI B,7
	CALL CURSOR
	LXI H,VKEY
	CALL DISPLAY
	CALL FLUSHCOM
LOOP:	MVI A,11
	MVI B,7
	CALL CURSOR
LOOP01:	CALL KYREAD ; Scans kbd for a key, returns in A, if any.
	JC LOOP02 ; Carry = Fn key
	JNZ HDLKBD
	CALL HDLCOM ; check COM for incoming text
	JMP LOOP ; we should handle the line. For now just loop back
LOOP02: CPI 7 ; F8 Quit. A 0-7 = F1-F8
	JZ THEEND
	CPI 0 ; F1 PING
	JZ  PING
	CPI 1 ; F2 Freq
	JZ  DOFREQ
	CPI 2 ; F3 BW
	JZ  DOBW
	CPI 3 ; F4 SF
	JZ  DOSF
	CPI 4 ; F5 STATUS
	JZ  DOSTATUS
	CPI 5 ; F6 autoping
	JZ  DOAP
	CPI 6 ; F7 Message
	JZ  SENDLINE
LOOP03:	CALL NOVALID
	JMP LOOP01

CURSOR:	; a = x. b = y
	PUSH H ; preserves HL
	LXI H, CSRX
	MOV M, A
	LXI H, CSRY
	MOV M, B ; cursor X, Y
	POP H ; retrieves HL
	RET

HDLKBD:	
	CPI 'Q ; 0x51
	JZ THEEND
	CPI 'q ; 0x71
	JZ THEEND
	CPI 'P ; 0x50
	JZ  PING
	CPI 'p ; 0x70
	JZ  PING
	CPI 'F ; 0x46
	JZ  DOFREQ
	CPI 'f ; 0x66
	JZ  DOFREQ
	CPI 'B ; 0x42
	JZ  DOBW
	CPI 'b ; 0x62
	JZ  DOBW
	CPI 'S ; 0x53
	JZ  DOSF
	CPI 's ; 0x73
	JZ  DOSF
	CPI 'T ; 0x54
	JZ  DOSTATUS
	CPI 't ; 0x74
	JZ  DOSTATUS
	CPI 'A ; 0x41
	JZ DOAP
	CPI 'a ; 0x61
	JZ DOAP
	CPI 'M ; 0x4d
	JZ SENDLINE
	CPI 'm ; 0x6d
	JZ SENDLINE
	PUSH PSW
	CALL MYBEEP
	MVI A,1
	MVI B,6
	CALL CURSOR
	CALL ERAEOL
	POP PSW
	LXI H,DUNNO
	MOV M,A
	DCX H ; DUNNO0
	CALL DISPLAY
	JMP LOOP01

PING:	CALL CLEARSPACE
	LXI H,NPING
	CALL SNDSRL
	JMP LOOP

THEEND:	CALL CLSCOM
	JMP MENU

CLEARSPACE:	MVI A,1
	MVI B,5
	CALL CURSOR
	CALL ERAEOL
	MVI A,1
	MVI B,4
	CALL CURSOR
	CALL ERAEOL
	MVI A,1
	MVI B,3
	CALL CURSOR
	CALL ERAEOL
	MVI A,1
	MVI B,2
	CALL CURSOR
	CALL ERAEOL
	RET

SENDLINE:	CALL CLEARSPACE
	MVI A,1
	MVI B,2
	CALL CURSOR
	LXI H, ASKLINE
	CALL DISPLAY
	MVI A,1
	MVI B,3
	CALL CURSOR
	CALL INLIN ; Gets line from kbd, ended by RETURN. Stored at 0xF685.
	MVI A,1
	MVI B,2
	CALL CURSOR
	CALL ERAEOL
	MVI A,1
	MVI B,3
	CALL CURSOR
	CALL ERAEOL
	LXI H,0xF685
	MOV A,M
	CPI 0 ; if line is empty, go back to the menu
	JZ, LOOP00
	CALL SRLLINE ; send the line, prefixed with />
	JMP LOOP

NOVALID:	CALL MYBEEP
	MVI A,1
	MVI B,6
	CALL CURSOR
	CALL ERAEOL
	LXI H,DUNNO
	INX H
	INX H
	INX H
	INX H
	INX H
	INX H ; "not a valid choice."
	CALL DISPLAY
	RET

DOSTATUS:	CALL CLEARSPACE
	LXI H,MSTATUS
	CALL SNDSRL
	JMP LOOP

DOFREQ:	CALL CLS
	MVI A,10
	MVI B,1
	CALL CURSOR
	LXI H, FQMENU0
	CALL DISPLAY
	MVI A,1
	MVI B,8
	CALL CURSOR
	LXI H, FQMENU1
	CALL DISPLAY
DOFREQH:	CALL HDLCOM ; check COM for incoming text
	CALL KYREAD ; Scans kbd for a key, returns in A, if any.
	JC FREQ01
	JZ DOFREQH
	CPI 'B ; 0x42(ack)
	JZ LOOP00
	CPI 'b ; 0x62(ack)
	JZ LOOP00
	CPI 'Q ; 0x51(uit)
	JZ THEEND
	CPI 'q ; 0x71(uit)
	JZ THEEND
	CPI '0 ; 0x30
	JZ DOFREQ0
	CPI '1 ; 0x31
	JZ DOFREQ1
	CPI '2 ; 0x32
	JZ DOFREQ2
	CPI '3 ; 0x33
	JZ DOFREQ3
	CPI '4 ; 0x34
	JZ DOFREQ4
	CPI '5 ; 0x35
	JZ DOFREQ5
	CALL MYBEEP
	JMP DOFREQH
FREQ01:	CPI 0 ; F1 868. A 0-7 = F1-F8
	JZ  DOFREQ0
	CPI 1 ; F2 868.125
	JZ  DOFREQ1
	CPI 2 ; F3 868.250
	JZ  DOFREQ2
	CPI 3 ; F4 915
	JZ  DOFREQ3
	CPI 4 ; F5 915.125
	JZ  DOFREQ4
	CPI 5 ; F6 915.250
	JZ  DOFREQ5
	CPI 6 ; F7 Back
	JZ  LOOP00
	CPI 7 ; F8 Quit
	JZ THEEND
FREQ02:	CALL NOVALID
	JMP DOFREQH

DOFREQ0:	LXI H,FCHOICE0
	JMP DOFREQX
DOFREQ1:	LXI H,FCHOICE1
	JMP DOFREQX
DOFREQ2:	LXI H,FCHOICE2
	JMP DOFREQX
DOFREQ3:	LXI H,FCHOICE3
	JMP DOFREQX
DOFREQ4:	LXI H,FCHOICE4
	JMP DOFREQX
DOFREQ5:	LXI H,FCHOICE5
DOFREQX:	CALL SNDSRL
	JMP DOFREQH

DOBW:	CALL CLS
	MVI A,10
	MVI B,1
	CALL CURSOR
	LXI H, BWMENU0
	CALL DISPLAY
	MVI A,1
	MVI B,8
	CALL CURSOR
	LXI H, BWMENU1
	CALL DISPLAY
DOBWH:	CALL HDLCOM ; check COM for incoming text
	CALL KYREAD ; Scans kbd for a key, returns in A, if any.
	JC DOBWH1
	JZ DOBWH
	CPI 'B ; (0x42)
	JZ LOOP00
	CPI 'b ; (0x62)
	JZ LOOP00
	CPI 'q ; (0x51)
	JZ THEEND
	CPI 'q ; (0x71)
	JZ THEEND
	CPI '6 ; (0x36)
	JZ DOBW0
	CPI '7 ; (0x37)
	JZ DOBW1
	CPI '8 ; (0x38)
	JZ DOBW2
	CPI '9 ; (0x39)
	JZ DOBW3
	CALL MYBEEP
	JMP DOBWH
DOBWH1:	CPI 0 ; F1 62.5. A 0-7 = F1-F8
	JZ  DOBW0
	CPI 1 ; F2 125
	JZ  DOBW1
	CPI 2 ; F3 250
	JZ  DOBW2
	CPI 3 ; F4 500
	JZ  DOBW3
	CPI 6 ; F7 Back
	JZ  LOOP00
	CPI 7 ; F8 Quit
	JZ THEEND
	CALL NOVALID
	JMP DOBWH

DOBW0:	LXI H,BCHOICE0
	JMP DOBWX
DOBW1:	LXI H,BCHOICE1
	JMP DOBWX
DOBW2:	LXI H,BCHOICE2
	JMP DOBWX
DOBW3:	LXI H,BCHOICE3
DOBWX:	CALL SNDSRL
	JMP DOBWH

DOSF:	CALL CLS
	MVI A,10
	MVI B,1
	CALL CURSOR
	LXI H, SFMENU0
	CALL DISPLAY
	MVI A,1
	MVI B,8
	CALL CURSOR
	LXI H, SFMENU1
	CALL DISPLAY
DOSFH:	CALL HDLCOM ; check COM for incoming text
	CALL KYREAD ; Scans kbd for a key, returns in A, if any.
	JC DOSFH1
	JZ DOSFH ; No key? Loop back
	CPI 'B ; (0x42)
	JZ LOOP00
	CPI 'b ; (0x62)
	JZ LOOP00
	CPI 'q ; (0x51)
	JZ THEEND
	CPI 'q ; (0x71)
	JZ THEEND
	CPI '7 ; (0x37)
	JZ DOSF7
	CPI '8 ; (0x38)
	JZ DOSF8
	CPI '9 ; (0x39)
	JZ DOSF9
	CPI '0 ; (0x30)
	JZ DOSF10
	CPI '1 ; (0x39)
	JZ DOSF11
	CPI '2 ; (0x32)
	JZ DOSF12
	CALL MYBEEP
	JMP DOSFH
DOSFH1:	CPI 0 ; F1 7
	JZ  DOSF7
	CPI 1 ; F2 8
	JZ  DOSF8
	CPI 2 ; F3 9
	JZ  DOSF9
	CPI 3 ; F4 10
	JZ  DOSF10
	CPI 4 ; F5 11
	JZ  DOSF11
	CPI 5 ; F6 12
	JZ  DOSF12
	CPI 6 ; F7 Back
	JZ  LOOP00
	CPI 7 ; F8 Quit
	JZ THEEND
	CALL NOVALID
	JMP DOSFH

DOSF7:	LXI H,SFCHOICE7
	JMP DOSFX
DOSF8:	LXI H,SFCHOICE8
	JMP DOSFX
DOSF9:	LXI H,SFCHOICE9
	JMP DOSFX
DOSF10:	LXI H,SFCHOICE10
	JMP DOSFX
DOSF11:	LXI H,SFCHOICE11
	JMP DOSFX
DOSF12:	LXI H,SFCHOICE12
DOSFX:	CALL SNDSRL
	JMP DOSFH

DOAP:	CALL CLS
	MVI A,10
	MVI B,1
	CALL CURSOR
	LXI H, APMENU0
	CALL DISPLAY
	MVI A,1
	MVI B,8
	CALL CURSOR
	LXI H, APMENU1
	CALL DISPLAY
DOAPH:	CALL HDLCOM ; check COM for incoming text
	CALL KYREAD ; Scans kbd for a key, returns in A, if any.
	JC DOAPH1
	JZ DOAPH
	CPI 'B ; (0x42)
	JZ LOOP00
	CPI 'b ; (0x62)
	JZ LOOP00
	CPI 'q ; (0x51)
	JZ THEEND
	CPI 'q ; (0x71)
	JZ THEEND
	CPI '0 ; (0x30)
	JZ DOAP10
	CPI '1 ; (0x39)
	JZ DOAP1
	CPI '2 ; (0x32)
	JZ DOAP2
	CPI '5 ; (0x35)
	JZ DOAP5
	CPI 'O ; (0x4f)
	JZ DOAPO
	CPI 'O ; (0x6f)
	JZ DOAPO
	CALL MYBEEP
	JMP DOAPH
DOAPH1:	CPI 0 ; F1 OFF
	JZ  DOAPO
	CPI 1 ; F2 1 mn
	JZ  DOAP1
	CPI 2 ; F3 2 mn
	JZ  DOAP2
	CPI 3 ; F4 5 mn
	JZ  DOAP5
	CPI 4 ; F5 10 mn
	JZ  DOAP10
	CPI 6 ; F7 Back
	JZ  LOOP00
	CPI 7 ; F8 Quit
	JZ THEEND
	CALL NOVALID
	JMP DOAPH


DOAP10:	LXI H,APCHOICE10
	JMP DOAPX
DOAP1:	LXI H,APCHOICE1
	JMP DOAPX
DOAP2:	LXI H,APCHOICE2
	JMP DOAPX
DOAP5:	LXI H,APCHOICE5
	JMP DOAPX
DOAPO:	LXI H,APCHOICEO
DOAPX:	CALL SNDSRL
	JMP DOAPH

MYBEEP:	LXI D, 4433 ; C# octave 2
	MVI B, 5 ; 5th of a second
	CALL MUSIC
	LXI D, 8866 ; C# octave 2
	MVI B, 5 ; 5th of a second
	CALL MUSIC
	LXI D, 4433 ; C# octave 2
	MVI B, 5 ; 5th of a second
	JMP MUSIC

INITSRL:	CALL CLSCOM
	MVI H,9
	MVI L, 1CH ; 0b11100 = 8N1
	SETC ; FOR RS232 <--- Very important!
	CALL INZCOM ; init com
	MVI A,35
	MVI B,1
	CALL CURSOR
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
	LXI H,VHM0
	CALL SNDSRL
	RET

SNDSRL:	PUSH H
	IN 0xC8 ; before you use the UART, it is good practice to clear the UART receiver buffer
	; register with an input from port C8. This is done, for example, at 6CE5.
	MVI A,1
	MVI B,6
	CALL CURSOR
	CALL ERAEOL
	LXI H,VSENDING
	CALL DISPLAY
	POP H
SNDSRL0:	MOV A,M
	CPI 0 ; done?
	JZ SNDSRL1
	CALL SD232C
	MOV A,M
	CALL LCD ; output the string also to LCD
	INX H
	JMP SNDSRL0
SNDSRL1:	MVI A, 10 ; Send LF
	CALL SD232C
	RET ; and return

SRLLINE:	IN 0xC8 ; before you use the UART, it is good practice to clear the UART receiver buffer
	; register with an input from port C8. This is done, for example, at 6CE5.
	PUSH H ; Address of the buffer where the line is. Probably 0xF685, but who knows?
	MVI A,1
	MVI B,3
	CALL CURSOR
	CALL ERAEOL
	LXI H,VSENDING
	CALL DISPLAY
	MVI A,0x2f ; /
	CALL LCD
	MVI A,0x2f ; /
	CALL SD232C
	MVI A,0x3E ; >
	CALL LCD
	MVI A,0x3E ; >
	CALL SD232C
	POP H
	JMP SNDSRL0 ; Now go to the regular SNDSRL routine to send the line.
	

FLUSHCOM:	IN 0xC8 ; before you use the UART, it is good practice to clear the UART receiver buffer
	; register with an input from port C8. This is done, for example, at 6CE5.
FLUSHCOM0:	CALL RCVX ; Checks RS232 queue. Returns number in A.
	RZ
	CALL RV232C ; Read the next char
	RZ
	JMP FLUSHCOM0 ; Loop until the buffer is empty

HDLCOM:	CALL RCVX ; Checks RS232 queue. Returns number in A.
	RZ
	PUSH PSW ; Save # of chars
HDLCOM0:	POP PSW
	CPI 0
	RZ ; we read all the chars available
	SUI 1
	PUSH PSW
	CALL RV232C ; Z flag set if OK, reset if error (PE, PF, OF).
	JC HDLCOM2 ; C flag set if BREAK pressed. Need to POP PSW before returning
	CPI 10
	JZ HDLCOM1 ; we have a line if it's LF
	CPI 13
	JZ HDLCOM0 ; skip if it's CR
	CPI 0x7E
	JP HDLCOM0 ; skip if it's not ASCII
	LXI D, POSX
	LHLX ; HL=(DE)
	LXI B, MYBUFFER
	PUSH H
	DAD B ; HL = MYBUFFER + POSX
	MOV M,A
	INX H
	MVI A,0
	MOV M,A ; Terminate the string
	POP H
	LXI D, POSX
	INX H
	SHLX ; (DE) = HL
	JMP HDLCOM0
HDLCOM1:	MVI A,1
	MVI B,5
	CALL CURSOR
	CALL ERAEOL
	MVI A,1
	MVI B,6
	CALL CURSOR
	CALL ERAEOL
	MVI A,1
	MVI B,7
	CALL CURSOR
	CALL ERAEOL ; Cursor is set for now at 1,7 for "info msg"
	LXI H,MYBUFFER ; if first 2 chars are 'ID' display on line 6 & 7
	MOV A,M
	CPI 'I ; (0x49)
	JNZ HDLCOM3
	INX H
	MOV A,M
	CPI 'D ; '(0x44)
	JNZ HDLCOM3
	MVI A,1
	MVI B,5
	CALL CURSOR
	LXI H,INCOMING
	CALL DISPLAY
	MVI A,1
	MVI B,6
	CALL CURSOR ; Incoming message:
HDLCOM3:	LXI H,MYBUFFER
	CALL DISPLAY
	LXI D, POSX
	LXI H,0
	SHLX ; (DE) = HL
	; Reset the buffer index
	JMP HDLCOM0 ; TODO: deal with the line's content
	; Like decide if it's a message or something less important.
	; On the BastWAN format the messages it sends with a prefix
	; S- System info. M- Incoming Message. etc
HDLCOM2: POP PSW
	RET

VGREET:	DS "LORA MESSENGER"
	DB 0
MAINFNMENU:	DS "PING Freq  BW   SF  Stat Auto  Msg Quit"
	DB 0
VKEY:	DS "Select Menu: "
	DB 0
NPING: DS "/p"
	DB 0
SRLGREET:	DS "/DNT102"
	DB 0
VHM0:	DS "/HM0"
	DB 0
VSENDING:	DS "Sending: "
	DB 0

FQMENU0:	DS "Select frequency:"
	DB 0
FQMENU1:	DS " 868 .125 .250  915 .125 .250 Back Quit"
	DB 0
FCHOICE0: DS "/FQ868"
	DB 0
FCHOICE1: DS "/FQ868.125"
	DB 0
FCHOICE2: DS "/FQ868.25"
	DB 0
FCHOICE3: DS "/FQ915"
	DB 0
FCHOICE4: DS "/FQ915.125"
	DB 0
FCHOICE5: DS "/FQ915.25"
	DB 0

BWMENU0:	DS "Select Bandwidth:"
	DB 0
BWMENU1:	DS "62.5  125  250  500           Back Quit"
	DB 0
BCHOICE0: DS "/BW6"
	DB 0
BCHOICE1: DS "/BW7"
	DB 0
BCHOICE2: DS "/BW8"
	DB 0
BCHOICE3: DS "/BW9"
	DB 0

SFMENU0:	DS "Select Spreading Factor:"
	DB 0
SFMENU1:	DS "  7    8    9   10   11   12  Back Quit"
	DB 0
SFCHOICE7: DS "/SF7"
	DB 0
SFCHOICE8: DS "/SF8"
	DB 0
SFCHOICE9: DS "/SF9"
	DB 0
SFCHOICE10: DS "/SF10"
	DB 0
SFCHOICE11: DS "/SF11"
	DB 0
SFCHOICE12: DS "/SF12"
	DB 0

MSTATUS: DS "/ST"
	DB 0

APMENU0:	DS "Auto PING:"
	DB 0
SFMENU1:	DS "  7    8    9   10   11   12  Back Quit"
APMENU1:	DS " Off  1mn  2mn  5mn  10mn     Back Quit"
	DB 0

APCHOICEO: DS "/AP0"
	DB 0
APCHOICE1: DS "/AP1"
	DB 0
APCHOICE2: DS "/AP2"
	DB 0
APCHOICE5: DS "/AP5"
	DB 0
APCHOICE10: DS "/AP10"
	DB 0

ASKLINE: DS "Enter text to send: "
	DB 0

INCOMING: DS "Incoming message:"
	DB 0

DUNNO0:	DS "`"
DUNNO: DB 0
	DS "` is not a valid choice."
	DB 0
POSX:	DB 0,0
MYBUFFER:	DB 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
			DB 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
			DB 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
