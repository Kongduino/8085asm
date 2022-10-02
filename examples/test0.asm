	ORG 0xf024
BEGIN:	CALL INITSRL
LOOP00:	CALL CLS ; when a refresh of the menu is needed
	; CALL HOME
	MVI A,10
	MVI B,1
	CALL CURSOR ; cursor 10,1
	LXI H, VGREET
	CALL DISPLAY
	MVI A,1
	MVI B,4
	CALL CURSOR ; cursor 1,4
	LXI H,VKEY
	CALL DISPLAY
	CALL FLUSHCOM
LOOP:	MVI A,11
	MVI B,4
	CALL CURSOR ; cursor 11,4
LOOP01:	CALL KYREAD ; Scans kbd for a key, returns in A, if any.
	JNZ HDLKBD
	CALL HDLCOM ; check COM for incoming text
	JMP LOOP ; we should handle the line. For now just loop back

CURSOR:	; a = x. b = y
	PUSH H ; preserve HL
	LXI H, CSRX
	MOV M, A
	LXI H, CSRY
	MOV M, B ; cursor X, Y
	POP H ; retrieve HL
	RET

HDLKBD:	CPI 81 ; Q
	JZ THEEND
	CPI 113 ; q
	JZ THEEND
	CPI 80 ; P
	JZ  PING
	CPI 112 ; P
	JZ  PING
	CPI 0x46 ; F
	JZ  DOFREQ
	CPI 0x66 ; f
	JZ  DOFREQ
	CPI 0x42 ; B
	JZ  DOBW
	CPI 0x62 ; b
	JZ  DOBW
	CPI 0x53 ; S
	JZ  DOSF
	CPI 0x73 ; s
	JZ  DOSF
	CPI 0x54 ; T
	JZ  DOSTATUS
	CPI 0x74 ; T
	JZ  DOSTATUS
	CALL MYBEEP
	JMP LOOP01

PING:	MVI A,1
	MVI B,8
	CALL CURSOR ; cursor 1,8
	CALL ERAEOL
	MVI A,1
	MVI B,7
	CALL CURSOR ; cursor 1,7
	CALL ERAEOL
	MVI A,1
	MVI B,6
	CALL CURSOR ; cursor 1,6
	CALL ERAEOL
	LXI H,NPING
	CALL SNDSRL
	JMP LOOP

THEEND:	CALL CLSCOM
	JMP MENU

SENDLINE:	MVI A,1
	MVI B,8
	CALL CURSOR ; cursor 1,8
	CALL ERAEOL
	MVI A,1
	MVI B,7
	CALL CURSOR ; cursor 1,7
	CALL ERAEOL
	MVI A,1
	MVI B,6
	CALL CURSOR ; cursor 1,6
	CALL ERAEOL
	LXI H, ASKLINE
	CALL DISPLAY
	MVI A,1
	MVI B,7
	CALL CURSOR ; cursor 1,7
	CALL INLIN ; Gets line from kbd, ended by RETURN. Stored at 0xF685.
	LXI H,0xF685
	MOV A,M
	CPI 0 ; if line is empty, go back to the menu
	JZ, LOOP00
	CALL SRLLINE ; send the line, prefixed with />
	JMP LOOP

DOSTATUS:	MVI A,1
	MVI B,8
	CALL CURSOR ; cursor 1,8
	CALL ERAEOL
	MVI A,1
	MVI B,7
	CALL CURSOR ; cursor 1,7
	CALL ERAEOL
	MVI A,1
	MVI B,6
	CALL CURSOR ; cursor 1,6
	CALL ERAEOL
	LXI H,MSTATUS
	CALL SNDSRL
	JMP LOOP

DOFREQ:	CALL CLS
	CALL HOME
	LXI H, FQMENU0
	CALL DISPLAY
	MVI A,1
	MVI B,2
	CALL CURSOR ; cursor 1,2
	LXI H, FQMENU1
	CALL DISPLAY
	MVI A,1
	MVI B,3
	CALL CURSOR ; cursor 1,2
	LXI H, FQMENU2
	CALL DISPLAY
DOFREQH:	CALL HDLCOM ; check COM for incoming text
	CALL KYREAD ; Scans kbd for a key, returns in A, if any.
	JZ DOFREQH
	CPI 0x42 ; B(ack)
	JZ LOOP00
	CPI 0x42 ; b(ack)
	JZ LOOP00
	CPI 0x51 ; q(uit)
	JZ MENU
	CPI 0x71 ; q(uit)
	JZ MENU
	CPI 0x30 ; 0
	JZ DOFREQ0
	CPI 0x31 ; 1
	JZ DOFREQ1
	CPI 0x32 ; 2
	JZ DOFREQ2
	CPI 0x33 ; 3
	JZ DOFREQ3
	CPI 0x34 ; 4
	JZ DOFREQ4
	CPI 0x35 ; 5
	JZ DOFREQ5
	CALL MYBEEP
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
	CALL HOME
	LXI H, BWMENU
	CALL DISPLAY
	CALL HOME
DOBWH:	CALL HDLCOM ; check COM for incoming text
	CALL KYREAD ; Scans kbd for a key, returns in A, if any.
	JZ DOBWH
	CPI 0x42 ; B(ack)
	JZ LOOP00
	CPI 0x62 ; b(ack)
	JZ LOOP00
	CPI 0x51 ; q(uit)
	JZ MENU
	CPI 0x71 ; q(uit)
	JZ MENU
	CPI 0x36 ; 6
	JZ DOBW0
	CPI 0x37 ; 7
	JZ DOBW1
	CPI 0x38 ; 8
	JZ DOBW2
	CPI 0x39 ; 9
	JZ DOBW3
	CALL MYBEEP
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
	CALL HOME
	LXI H, SFMENU
	CALL DISPLAY
	MVI A,1
	MVI B,2
	CALL CURSOR ; cursor 1,2
	LXI H, SFMENU0
	CALL DISPLAY
DOSFH:	CALL HDLCOM ; check COM for incoming text
	CALL KYREAD ; Scans kbd for a key, returns in A, if any.
	JZ DOSFH ; No key? Loop back
	CPI 0x42 ; B(ack)
	JZ LOOP00
	CPI 0x62 ; b(ack)
	JZ LOOP00
	CPI 0x51 ; q(uit)
	JZ MENU
	CPI 0x71 ; q(uit)
	JZ MENU
	CPI 0x36 ; 6
	JZ DOSF6
	CPI 0x37 ; 7
	JZ DOSF7
	CPI 0x38 ; 8
	JZ DOSF8
	CPI 0x39 ; 9
	JZ DOSF9
	CPI 0x30 ; 0
	JZ DOSF10
	CPI 0x39 ; 1
	JZ DOSF11
	CPI 0x32 ; 2
	JZ DOSF12
	CALL MYBEEP
	JMP DOSFH

DOSF6:	LXI H,SFCHOICE6
	JMP DOSFX
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
	MVI A,29
	MVI B,1
	CALL CURSOR ; cursor 29,1
	LXI H, APMENU
	CALL DISPLAY
	CALL HOME
DOAPH:	CALL HDLCOM ; check COM for incoming text
	CALL KYREAD ; Scans kbd for a key, returns in A, if any.
	JZ DOAPH
	CPI 0x42 ; B(ack)
	JZ LOOP00
	CPI 0x62 ; b(ack)
	JZ LOOP00
	CPI 0x51 ; q(uit)
	JZ MENU
	CPI 0x71 ; q(uit)
	JZ MENU
	CPI 0x30 ; 0
	JZ DOAP10
	CPI 0x39 ; 1
	JZ DOAP1
	CPI 0x32 ; 2
	JZ DOAP2
	CPI 0x35 ; 5
	JZ DOAP5
	CPI 0x4f ; O
	JZ DOAPO
	CPI 0x6f ; O
	JZ DOAPO
	CALL MYBEEP
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
	CALL CURSOR ; cursor 35,1
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
	MVI B,6
	CALL CURSOR ; cursor 1,6
	CALL ERAEOL
	LXI H,VSENDING
	CALL DISPLAY
	POP H
SNDSRL0:	MOV A,M
	CPI 0
	RZ
	CALL SD232C
	MOV A,M
	CALL LCD ; output the string also to LCD
	INX H
	JMP SNDSRL0

SRLLINE:	IN 0xC8 ; before you use the UART, it is good practice to clear the UART receiver buffer
	; register with an input from port C8. This is done, for example, at 6CE5.
	PUSH H ; Address of the buffer where the line is. Probably 0xF685, but who knows?
	MVI A,1
	MVI B,6
	CALL CURSOR ; cursor 1,6
	CALL ERAEOL
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
	; MVI A,1
	; MVI B,5
	; CALL CURSOR ; cursor 1,5
	; CALL ERAEOL
	; LXI H,NUMCHARS
	; CALL DISPLAY
	; POP PSW
	; PUSH PSW
	; MOV L,A ; Prints the number of chars available on line 5
	; MVI H,0
	; CALL PRTINT
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
HDLCOM1:	LXI H,POSX ; reset POSX
	MVI A,1
	MOV M,A
	LXI H,POSY ; reset POSY
	MVI A,6
	MOV M,A
	LXI D, CSRY
	LXI H,0x0106
	SHLX ; (DE) = HL
	; cursor x 1 y 6 HOPEFULLY
	CALL ERAEOL
	LXI D, CSRY
	LXI H,0x0107
	SHLX ; (DE) = HL
	; cursor x 1 y 7 HOPEFULLY
	CALL ERAEOL
	LXI H,MYBUFFER
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
	DB 13,10
	DS "(P)ING (F)REQUENCY (B)W (S)F"
	DB 13,10
	DS "S(T)ATUS (A)uto PING (Q)uit"
	DB 0
VKEY:	DS "ENTER KEY:"
	DB 0
NPING: DS "/p"
	DB 10,0
SRLGREET:	DS "/DNT102"
	DB 10,0
VSENDING:	DS "Sending: "
	DB 0

FQMENU0:	"Enter freq:"
	DB 0
FQMENU1:	DS "(0)868 (1)868.125 (2)868.250"
	DB 0
FQMENU2:	DS "(3)915 (4)915.125 (5)915.250 (B)ack"
	DB 0
FCHOICE0: DS "/FQ868"
	DB 10, 0
FCHOICE1: DS "/FQ868.125"
	DB 10, 0
FCHOICE2: DS "/FQ868.25"
	DB 10, 0
FCHOICE3: DS "/FQ915"
	DB 10, 0
FCHOICE4: DS "/FQ915.125"
	DB 10, 0
FCHOICE5: DS "/FQ915.25"
	DB 10, 0

BWMENU:	"Enter bandwidth:"
	DB 13,10
	DS "(6)62.5 KHz (7)125 KHz (8)250 KHz"
	DB 13,10
	DS "(9)500 (B)ack (Q)uit"
	DB 0
BCHOICE0: DS "/BW6"
	DB 10, 0
BCHOICE1: DS "/BW7"
	DB 10, 0
BCHOICE2: DS "/BW8"
	DB 10, 0
BCHOICE3: DS "/BW9"
	DB 10, 0

SFMENU:	"Enter Spreading Factor:"
	DB 0
SFMENU0:	DS "(6) (7) (8) (9) 1(0) 1(1) 1(2)"
	DB 13,10
	DS "(B)ack (Q)uit"
	DB 0
SFCHOICE6: DS "/SF6"
	DB 10, 0
SFCHOICE7: DS "/SF7"
	DB 10, 0
SFCHOICE8: DS "/SF8"
	DB 10, 0
SFCHOICE9: DS "/SF9"
	DB 10, 0
SFCHOICE10: DS "/SF10"
	DB 10, 0
SFCHOICE11: DS "/SF11"
	DB 10, 0
SFCHOICE12: DS "/SF12"
	DB 10, 0

MSTATUS: DS "/ST"
	DB 0

NUMCHARS: DS "Chars in COM buffer: "
	DB 0

APMENU:	"Auto PING:"
  DB 13,10
	DS "(O)ff (1) mn (2) mn (5) mn 1(0) mn"
	DB 0
APCHOICEO: DS "/AP0"
	DB 10, 0
APCHOICE1: DS "/AP1"
	DB 10, 0
APCHOICE2: DS "/AP2"
	DB 10, 0
APCHOICE5: DS "/AP5"
	DB 10, 0
APCHOICE10: DS "/AP10"
	DB 10, 0

POSX:	DB 0,0
MYBUFFER: DB 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	DB 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
