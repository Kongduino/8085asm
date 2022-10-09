typedef struct {
  int value;
  char name[8];
} Hex2Label;

Hex2Label mylabels[] = {
  {0x5A58, "DISPLAY"},
  {0x4B44, "LCD"},
  {0x12CB, "CHGET"},
  {0x5797, "MENU"},
  {0x744C, "UNPLOT"},
  {0x744D, "PLOT"},
  {0x4D7C, "POSIT"},
  {0x4270, "ESCA"},
  {0x4222, "CRLF"},
  {0x422D, "HOME"},
  {0x4231, "CLS"},
  {0x4235, "SETSYS"},
  {0x423A, "RSTSYS"},
  {0x423F, "LOCK"},
  {0x4244, "UNLOCK"},
  {0x4249, "CURSON"},
  {0x424E, "CUROFF"},
  {0x4253, "DELLIN"},
  {0x4258, "INSLIN"},
  {0x425D, "ERAEOL"},
  {0x4269, "ENTREV"},
  {0x426E, "EXTREV"},
  {0x7242, "KYREAD"},
  {0x12CB, "CHGET"},
  {0x5A7C, "STFNK"},
  {0x5A79, "CLRFNK"},
  {0x5A9E, "FNKSB"},
  {0x42A5, "DSPFNK"},
  {0x428A, "ERAFNK"},
  {0x13DB, "CHSNS"},
  {0x7270, "KEYX"},
  {0x7283, "BRKCHK"},
  {0x4644, "INLIN"},
  {0x6D3F, "PRINTR"},
  {0x1470, "PNOTAB"},
  {0x4B55, "PRTTAB"},
  {0x1E5E, "PRTLCD"},
  {0x52BB, "DISC"},
  {0x52D0, "CONN"},
  {0x532D, "DIAL"},
  {0x6D6D, "RCVX"},
  {0x6D7E, "RV232C"},
  {0x6E32, "SD232C"},
  {0x6E0B, "SENDCQ"},
  {0x6E1E, "SENDCS"},
  {0x6EEF, "CARDET"},
  {0x6E75, "BAUDST"},
  {0x6EA6, "INZCOM"},
  {0x6ECB, "CLSCOM"},
  {0x702A, "DATAR"},
  {0x6F5B, "DATAW"},
  {0x14A8, "CTON"},
  {0x14AA, "CTOF"},
  {0x14B0, "CASIN"},
  {0x14C1, "CSOUT"},
  {0x6F46, "SYNCW"},
  {0x6F85, "SYNCR"},
  {0x220F, "MAKTXT"},
  {0xFC93, "FILNAM"},
  {0x5AA9, "CHKDC"},
  {0x5AE3, "GTXTTB"},
  {0x1FBE, "KILASC"},
  {0x6B61, "INSCHR"},
  {0x6B6D, "MAKHOL"},
  {0x6B9F, "MASDEL"},
  {0x6CD6, "INITIO"},
  {0x6CE0, "IOINIT"},
  {0x72C5, "MUSIC"},
  {0x190F, "TIME"},
  {0x192F, "DATE"},
  {0x1962, "DAY"},
  {0xF933, "SECS0"},
  {0xF934, "SECS1"},
  {0xF639, "CSRY"},
  {0xF63A, "CSRX"},
  {0xFE00, "BEGLCD"},
  {0xFF40, "ENDLCD"},
  {0xF65B, "STAT"},
  {0xF962, "DIRTBL"},
  {0x5f2f,"PAUSE"},
  {0x39D4,"PRTINT"},
  {0x4229,"BEEP"},
  {0x0fe8,"GETUC"},
  {0x0fe9,"TOUPPER"},
  {0x0fe9,"UPCAS"},
  {0X4644,"GETLNP"},
  {0X463e,"GETLN"},
  {0x5d46,"CHKCHR"},
  {0xf685,"INBUF"},
  {0X427C,"POSIT"},
  {0X59C9,"BARPOS"},
  {0X59E5,"BARCUR"},
  {0X5A18,"DTLINE"},
  {0X5A15,"TDDPT"},
  {0X7EAC,"PFRE"},
  {0X5D64,"KEYWTU"},
  {0X729F,"BRKCHK"},
  {0X72B1,"SCANSP"},
  {0X42A8,"DSPFNK"},
  {0X5B46,"CSFKEY"},
  {0XF80A,"BASFKY"},
  {0X6C9C,"BK2SK"},
  {0XF650,"FKCTRL"},
  {0XFAAD,"LBENBL"},
  {0X6E3A,"SNDCOM"},
  {0XFF,"ENDDIR"},
  {0XF9BA,"FDIR"},
  {0X4C0B,"PRSNAM"},
  {0X20AF,"FNDFIL"},
  {0X5AAB,"CHKFN"},
  {0X59AD,"MKPNAM"},
  {0X2146,"DIROK"},
  {0X20D5,"NXTDIR"},
  {0X20EC,"FREDIR"},
  {0X14ED,"POPALL"},
  {0X7D33,"BOOT"},
  {0X143F,"PWRRES"},
  {0X3469,"BLKMV"},
  {0X08EC,"LINNUM"},
  {0X1297,"BASEXPR"},
  {0X344A,"PRGADD"},
  {0X1BB1,"RSTDOG"},
  {0X5DEE,"TEXT"},
  {0X6C49,"BASIC"},
  {0X5146,"TELCOM"},
  {0X5B68,"ADDRSS"},
  {0X5B6F,"SCHEDL"},
  {0X5B74,"SABASE"},
  {0XFFD9,"ADSCFILE"},
  {0X5F71,"TXTENT"},
  {0XF6E7,"FILOFF"},
  {0XA0,"PHONEPORT"},
  {0XB0,"PIOCMD"},
  {0XB1,"PIOA"},
  {0XB2,"PIOB"},
  {0XB3,"PIOC"},
  {0XB4,"PIOTL"},
  {0XB5,"PIOTH"},
  {0XC0,"UARTDAT"},
  {0XD0,"UARTCFG"},
  {0XE0,"KEYPORT"},
  {0XF0,"LCDPORT"},
  {0XFB9D,"STKTOP"},
  {0XF657,"PTIMER"},
  {0XF932,"POWERFLAG"},
  {0XF88C,"PASTEPTR"},
  {0XF88A,"SHPRTPTR"},
  {0XF650,"BRKDIS"},
  {0XF5EF,"MAXRAM"},
  {0XF5F4,"HIMEM"},
  {0XF99A,"FREBAS"},
  {0XFC82,"MAXFILE"},
  {0XF9A5,"NEXTTXT"},
  {0XFBAE,"DOSTART"},
  {0XFBB0,"COSTART"},
  {0XFBB2,"VARSTART"},
  {0XFBB4,"ARRSTART"},
  {0XFBB6,"FRESTART"},
  {0XF88C,"FILEEND"},
  {0XFAC0,"SYSRAM"},
  {0XFADA,"HOOK"}
};

int numLabels = 79;
