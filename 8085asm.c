/* ########################################################################
   8085asm - Simple 8085 assembler and disassembler
   ########################################################################
   Copyright (c):	2009 Luis Claudio Gamboa Lopes
                  	2022 Kongduino for this version
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
   For e-mail suggestions:  lcgamboa@yahoo.com for the original version
                            kongduino@protonmail.com for this version
   ########################################################################
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <ctype.h>
#include "8085asm.h"

int lc = 1;
unsigned short doCompile();

void ucase (char * str) {
  int i;
  if (str != NULL) {
    for (i = 0; i < strlen(str); i++)
      str[i] = toupper(str[i]);
  }
}

int addr = 0;
int addi = 0;
unsigned char prg[256];
unsigned long labelsc = 0;
int pass;
int memc;
unsigned char sum;
unsigned char nb;
unsigned short iaddr;
char values[100];
char *fnamep;
FILE *fout;
FILE *fout1;
FILE *fout2;
FILE *fout3;
FILE *fout4;
FILE* fin;
char fname[256];
char fname0[256];
char fname1[256];
char fname2[256];
char fname3[256];
char fname4[256];
char relocate = 'Y';

int parseNumber(char *arg, char *line) {
  int n0 = 0, n1 = 0, n2 = 0, myValue = 0;
  if (arg[0] == '\'') {
    // ':'
    int n = sscanf(line, "%*s '%c", &myValue);
    // printf("n = %d, myValue = %d\n", n, myValue);
    if (n == 1) return myValue;
	n = sscanf(line, "%*s: %*s '%c", &myValue);
    if (n == 1) return myValue;
    printf("sscanf failed.\n");
  }
  if (arg[0] == '0' && (arg[1] == 'x' || arg[1] == 'X'))
    n0 = 1;
  if (arg[strlen(arg) - 1] == 'h' || arg[strlen(arg) - 1] == 'H')
    n1 = 1;
  if (arg[0] == '0' && (arg[1] == 'b' || arg[1] == 'B'))
    n2 = 1;
  if (n0 == 1) {
    sscanf(arg, "%X", &myValue);
    // printf("[n0] myValue = 0x%04x\n", myValue);
    return myValue;
  }
  if (n1 == 1 && n0 == 0) {
    arg[strlen(arg) - 1] = 0;
    sscanf(arg, "%X", &myValue);
    // printf("[n1] myValue = 0x%04x\n", myValue);
    return myValue;
  }
  if (n1 == 0 && n0 == 0 && n2 == 1) {
    unsigned int z, y = strlen(arg);
    myValue = 0;
    for (z = 2; z < y; z++) {
      if (arg[z] != '0' && arg[z] != '1') {
        // printf("Bad format!");
        break;
      }
      myValue = (myValue << 1) | (arg[z] == '0' ? 0 : 1);
    }
    // printf("[n2] myValue = 0x%04x\n", myValue);
    return myValue;
  }
  unsigned int z, y = strlen(arg);
  myValue = 0;
  for (z = 0; z < y; z++) {
    if (arg[z] < '0' || arg[z] > '9') {
      printf("Bad format! %s\n", line);
      return -1;
    }
    myValue = (myValue * 10) + (arg[z] - '0');
  }
  // printf("[dec] myValue = 0x%04x\n", myValue);
  return myValue;
}

int parsearg(char* arg, char * line) {
  unsigned long i;
  if (arg == NULL) {
    printf("line %i error!!!!\n%s\n", lc + 1, line);
    exit(-1);
  };
  if(arg[0] != '\'' && arg[0]>'9') {
    for (i = 0; i < labelsc; i++) {
      if (strcmp(labels[i].nome, arg) == 0) {
        return labels[i].value;
      };
    }
    printf("%s is not in labels. [pass = %d]. %s\n", arg, pass, labels[258]);
  }
  // sscanf(arg, "%X", &i);
  // return i;
  return parseNumber(arg, line);
};

int parse(char * line) {
  int i = 0;
  int t;
  char sline[256];
  char* label;
  char* men;
  char* arg1;
  char* arg2;
  int err;
  addi = 0;
  if ((line[0] == '\n') || (line[0] == '\r')) return 0;
  label = strtok(line, ";\n\r");
  strcpy(sline, label);
  if ((sline[0] != ' ') && (sline[0] != '\t')) {
    label = strtok(sline, " \t:, \n");
    men = strtok(NULL, " \t:, \n");
    if (pass == 1) {
      //printf("label=%s  %04X\n", label, addr);
      ucase(label);
      strcpy(labels[labelsc].nome, label);
      labels[labelsc].value = addr;
      // printf(" • New label: %s. labelsc = %d.\n", label, labelsc);
      labelsc++;
    }
  } else {
    label = NULL;
    men = strtok(sline, " \t:, \n");
  }
  // printf("men = %s\n", men);
  if (men == NULL)return 0;
  arg1 = strtok(NULL, " \t:, \n");
  arg2 = strtok(NULL, " \t:, \n");
  ucase(men);
  if (arg1 != NULL) ucase(arg1);
  if (arg2 != NULL) ucase(arg2);
  //printf("addr=%04X label=%s  opcode=%s  arg1=%s  arg2=%s\n", addr, label, men, arg1, arg2);
  err = 0;
  //busca opcodes
  do {
    if (strcmp(opcode[i].men, men) == 0) {
      if (opcode[i].nargs == 0) {
        addi = opcode[i].uargs;
        prg[0] = opcode[i].opc;
        if (pass == 2)
          switch (addi) {
            case 0:
              return 1;
            case 1:
              prg[1] = parsearg(arg1, line);
              return 1;
            case 2:
              t = parsearg(arg1, line);
              prg[1] = 0x00FF & t;
              prg[2] = (0xFF00 & t) >> 8;
              return 1;
          }
        return 1;
      } else {
        if (arg1 == NULL) {
          printf("line %i error!!!!\n%s\n", lc, line);
          exit(-1);
        }
        if (strcmp(opcode[i].arg1, arg1) == 0) {
          if (opcode[i].nargs == 1) {
            addi = opcode[i].uargs;
            prg[0] = opcode[i].opc;
            if (pass == 2)
              switch (addi) {
                case 0:
                  return 1;
                case 1:
                  prg[1] = parsearg(arg2, line);
                  return 1;
                case 2:
                  t = parsearg(arg2, line);
                  prg[1] = 0x00FF & t;
                  prg[2] = (0xFF00 & t) >> 8;
                  return 1;
              }
            return 1;
          } else {
            if (strcmp(opcode[i].arg2, arg2) == 0) {
              addi = opcode[i].uargs;
              prg[0] = opcode[i].opc;
              return 1;
            }
          }
        }
        err = 1;
      }
    } else {
      if (err) {
        printf("line %i error!\n%s\n", lc, line);
        exit(-1);
      }
    }
  } while (strcmp(opcode[i++].men, "ENDO") != 0);
  //busca pseudo
  if (strcmp("ORG", men) == 0) {
    addr = parsearg(arg1, line);
    // printf("org ===> 0x%04X\n", addr);
    return 0;
  };
  if ((label != NULL) && (pass == 1)) {
    if (strcmp("EQU", men) == 0) {
      sscanf(arg1, "%x", &i);
      labels[labelsc - 1].value = i;
      return 0;
    }
  }
  if (strcmp("DB", men) == 0) {
    addi = 0;
    prg[addi] = parsearg(arg1, line);
    if (arg2 != NULL) {
      addi++;
      prg[addi] = parsearg(arg2, line);
    }
    arg1 = strtok(NULL, " \t:, \n");
    while (arg1 != NULL) {
      ucase(arg1);
      addi++;
      prg[addi] = parsearg(arg1, line);
      arg1 = strtok(NULL, " \t:, \n");
    }
    return 1;
  }
  if (strcmp("DW", men) == 0) {
    addi = 0;
    unsigned int tb = parsearg(arg1, line);
    prg[addi] = (unsigned char)(tb & 0xFF);
    addi++;
    prg[addi] = (unsigned char)((tb & 0xFF00) >> 8);
    if (arg2 != NULL) {
      addi++;
      tb = parsearg(arg2, line);
      prg[addi] = (unsigned char)(tb & 0xFF);
      addi++;
      prg[addi] = (unsigned char)((tb & 0xFF00) >> 8);
    }
    arg1 = strtok(NULL, " \t:, \n");
    while (arg1 != NULL) {
      ucase(arg1);
      addi++;
      tb = parsearg(arg1, line);
      prg[addi] = (unsigned char)(tb & 0xFF);
      addi++;
      prg[addi] = (unsigned char)((tb & 0xFF00) >> 8);
      arg1 = strtok(NULL, " \t:, \n");
    }
    return 1;
  }
  if (strcmp("DS", men) == 0) {
    char tmp[32] = {0};
    int na = sscanf(line, "%*[^\"]\"%[^\"]", &tmp);
    if (na != 1) {
      printf("Error scanning line for DS string: `%s`. na = %d, tmp = `%s`\n", line, na, tmp);
      return -1;
    }
    int nx, ny = strlen(tmp);
    for (nx = 0; nx < ny; nx++) {
      // printf(" . Adding char %c like it was `DB `%02x`, \n", tmp[nx], tmp[nx]);
      prg[addi + nx] = tmp[nx];
    }
    addi += (ny - 1);
  }
  return 1;
}

int main(int argc, char** argv) {
  printf("################################\n");
  printf("# LCGamboa 8085 assembler 2022 #\n");
  printf("################################\n");
  char line[256];
  size_t optind = 1;
  while (optind < argc && argv[optind][0] == '-') {
    // printf("arg: %s\n", argv[optind]);
    switch (argv[optind][1]) {
      case 'X':
        // Do not relocate
        relocate = 'N';
        optind += 1;
        printf(" * Do not relocate. OK.\n");
      case 'L':
        // Relocate to alternate LCD buffer,
        // BEGLCD 0xFE00 to ENDLCD EQU 0XFF40, 320 bytes
        // Only if the code fits of course...
        relocate = 'L';
        optind += 1;
        printf(" * Relocate to LCD buffer, 0xFE00-0xFF40.\n");
    }
  }
  if (optind < argc) {
    strcpy(fname, argv[optind]);
  } else {
    printf("enter the file name: ");
    scanf("%s", fname);
    getchar();
  }
  printf(" * Opening file: %s\n", fname);
  fin = fopen(fname, "r");
  if (!fin) {
    printf(" ! Error opening file: %s\n", fname);
    return -1;
  }
  strcpy(fname0, fname);
  fnamep = strtok(fname, ".");
  strcpy(fname1, fnamep);
  strcat(fname1, ".copy.asm");
  strcpy(fname2, fnamep);
  strcat(fname2, ".map");
  strcpy(fname3, fnamep);
  strcat(fname3, ".co");
  strcpy(fname4, fnamep);
  strcat(fname4, ".do");
  printf(" * Saving %s:\n", fname2);
  fout = fopen(fname2, "w");
  if (!fout) {
    printf("Error opening file: %s\n", fname2);
    return -1;
  }
  fprintf(fout, "8085 Simple Assembler 2022\n\n");
  printf(" • Creating internal labels... ");
  strcpy(labels[labelsc].nome, "BAUDST"); labels[labelsc++].value = 0x6E75;
  strcpy(labels[labelsc].nome, "BEGLCD"); labels[labelsc++].value = 0xFE00;
  strcpy(labels[labelsc].nome, "ENDLCD"); labels[labelsc++].value = 0xFF40;
  strcpy(labels[labelsc].nome, "BRKCHK"); labels[labelsc++].value = 0x7283;
  strcpy(labels[labelsc].nome, "CARDET"); labels[labelsc++].value = 0x6EEF;
  strcpy(labels[labelsc].nome, "CASIN"); labels[labelsc++].value = 0x14B0;
  strcpy(labels[labelsc].nome, "CHGET"); labels[labelsc++].value = 0x12CB;
  strcpy(labels[labelsc].nome, "CHKDC"); labels[labelsc++].value = 0x5AA9;
  strcpy(labels[labelsc].nome, "CHSNS"); labels[labelsc++].value = 0x13DB;
  strcpy(labels[labelsc].nome, "CLRFNK"); labels[labelsc++].value = 0x5A79;
  strcpy(labels[labelsc].nome, "CLS"); labels[labelsc++].value = 0x4231;
  strcpy(labels[labelsc].nome, "CLSCOM"); labels[labelsc++].value = 0x6ECB;
  strcpy(labels[labelsc].nome, "CONN"); labels[labelsc++].value = 0x52D0;
  strcpy(labels[labelsc].nome, "CRLF"); labels[labelsc++].value = 0x4222;
  strcpy(labels[labelsc].nome, "CSOUT"); labels[labelsc++].value = 0x14C1;
  strcpy(labels[labelsc].nome, "CSRX"); labels[labelsc++].value = 0xF63A;
  strcpy(labels[labelsc].nome, "CSRY"); labels[labelsc++].value = 0xF639;
  strcpy(labels[labelsc].nome, "CTOF"); labels[labelsc++].value = 0x14AA;
  strcpy(labels[labelsc].nome, "CTON"); labels[labelsc++].value = 0x14A8;
  strcpy(labels[labelsc].nome, "CUROFF"); labels[labelsc++].value = 0x424E;
  strcpy(labels[labelsc].nome, "CURSON"); labels[labelsc++].value = 0x4249;
  strcpy(labels[labelsc].nome, "DATAR"); labels[labelsc++].value = 0x702A;
  strcpy(labels[labelsc].nome, "DATAW"); labels[labelsc++].value = 0x6F5B;
  strcpy(labels[labelsc].nome, "DATE"); labels[labelsc++].value = 0x192F;
  strcpy(labels[labelsc].nome, "DAY"); labels[labelsc++].value = 0x1962;
  strcpy(labels[labelsc].nome, "DELLIN"); labels[labelsc++].value = 0x4253;
  strcpy(labels[labelsc].nome, "DIAL"); labels[labelsc++].value = 0x532D;
  strcpy(labels[labelsc].nome, "DIRTBL"); labels[labelsc++].value = 0xF962;
  strcpy(labels[labelsc].nome, "DISC"); labels[labelsc++].value = 0x52BB;
  strcpy(labels[labelsc].nome, "DISPLAY"); labels[labelsc++].value = 0x5A58;
  strcpy(labels[labelsc].nome, "DSPFNK"); labels[labelsc++].value = 0x42A5;
  strcpy(labels[labelsc].nome, "ENTREV"); labels[labelsc++].value = 0x4269;
  strcpy(labels[labelsc].nome, "ERAEOL"); labels[labelsc++].value = 0x425D;
  strcpy(labels[labelsc].nome, "ERAFNK"); labels[labelsc++].value = 0x428A;
  strcpy(labels[labelsc].nome, "ESCA"); labels[labelsc++].value = 0x4270;
  strcpy(labels[labelsc].nome, "EXTREV"); labels[labelsc++].value = 0x426E;
  strcpy(labels[labelsc].nome, "FILNAM"); labels[labelsc++].value = 0xFC93;
  strcpy(labels[labelsc].nome, "FNKSB"); labels[labelsc++].value = 0x5A9E;
  strcpy(labels[labelsc].nome, "GTXTTB"); labels[labelsc++].value = 0x5AE3;
  strcpy(labels[labelsc].nome, "HOME"); labels[labelsc++].value = 0x422D;
  strcpy(labels[labelsc].nome, "INITIO"); labels[labelsc++].value = 0x6CD6;
  strcpy(labels[labelsc].nome, "INLIN"); labels[labelsc++].value = 0x4644;
  strcpy(labels[labelsc].nome, "INSCHR"); labels[labelsc++].value = 0x6B61;
  strcpy(labels[labelsc].nome, "INSLIN"); labels[labelsc++].value = 0x4258;
  strcpy(labels[labelsc].nome, "INZCOM"); labels[labelsc++].value = 0x6EA6;
  strcpy(labels[labelsc].nome, "IOINIT"); labels[labelsc++].value = 0x6CE0;
  strcpy(labels[labelsc].nome, "KEYX"); labels[labelsc++].value = 0x7270;
  strcpy(labels[labelsc].nome, "KILASC"); labels[labelsc++].value = 0x1FBE;
  strcpy(labels[labelsc].nome, "KYREAD"); labels[labelsc++].value = 0x7242;
  strcpy(labels[labelsc].nome, "LCD"); labels[labelsc++].value = 0x4B44;
  strcpy(labels[labelsc].nome, "LOCK"); labels[labelsc++].value = 0x423F;
  strcpy(labels[labelsc].nome, "MAKHOL"); labels[labelsc++].value = 0x6B6D;
  strcpy(labels[labelsc].nome, "MAKTXT"); labels[labelsc++].value = 0x220F;
  strcpy(labels[labelsc].nome, "MASDEL"); labels[labelsc++].value = 0x6B9F;
  strcpy(labels[labelsc].nome, "MENU"); labels[labelsc++].value = 0x5797;
  strcpy(labels[labelsc].nome, "MUSIC"); labels[labelsc++].value = 0x72C5;
  strcpy(labels[labelsc].nome, "PLOT"); labels[labelsc++].value = 0x744D;
  strcpy(labels[labelsc].nome, "PNOTAB"); labels[labelsc++].value = 0x1470;
  // strcpy(labels[labelsc].nome, "POSIT"); labels[labelsc++].value = 0x4D7C; Seems to be wrong See below
  strcpy(labels[labelsc].nome, "PRINTR"); labels[labelsc++].value = 0x6D3F;
  strcpy(labels[labelsc].nome, "PRTLCD"); labels[labelsc++].value = 0x1E5E;
  strcpy(labels[labelsc].nome, "PRTTAB"); labels[labelsc++].value = 0x4B55;
  strcpy(labels[labelsc].nome, "RCVX"); labels[labelsc++].value = 0x6D6D;
  strcpy(labels[labelsc].nome, "RSTSYS"); labels[labelsc++].value = 0x423A;
  strcpy(labels[labelsc].nome, "RV232C"); labels[labelsc++].value = 0x6D7E;
  strcpy(labels[labelsc].nome, "SD232C"); labels[labelsc++].value = 0x6E32;
  strcpy(labels[labelsc].nome, "SECS0"); labels[labelsc++].value = 0xF933;
  strcpy(labels[labelsc].nome, "SECS1"); labels[labelsc++].value = 0xF934;
  strcpy(labels[labelsc].nome, "SENDCQ"); labels[labelsc++].value = 0x6E0B;
  strcpy(labels[labelsc].nome, "SENDCS"); labels[labelsc++].value = 0x6E1E;
  strcpy(labels[labelsc].nome, "SETSER"); labels[labelsc++].value = 0x17E6;
  strcpy(labels[labelsc].nome, "SETSYS"); labels[labelsc++].value = 0x4235;
  strcpy(labels[labelsc].nome, "STAT"); labels[labelsc++].value = 0xF65B;
  strcpy(labels[labelsc].nome, "STFNK"); labels[labelsc++].value = 0x5A7C;
  strcpy(labels[labelsc].nome, "SYNCR"); labels[labelsc++].value = 0x6F85;
  strcpy(labels[labelsc].nome, "SYNCW"); labels[labelsc++].value = 0x6F46;
  strcpy(labels[labelsc].nome, "TIME"); labels[labelsc++].value = 0x190F;
  strcpy(labels[labelsc].nome, "UNLOCK"); labels[labelsc++].value = 0x4244;
  strcpy(labels[labelsc].nome, "PLOT"); labels[labelsc++].value = 0x744C;
  strcpy(labels[labelsc].nome, "UNPLOT"); labels[labelsc++].value = 0x744D;
  strcpy(labels[labelsc].nome, "SETCUR"); labels[labelsc++].value = 0x7440;
  strcpy(labels[labelsc].nome, "PAUSE"); labels[labelsc++].value = 0x5f2f; // DEBUG413
  strcpy(labels[labelsc].nome, "PRTINT"); labels[labelsc++].value = 0x39D4; // DEBUG413
  strcpy(labels[labelsc].nome, "BEEP"); labels[labelsc++].value = 0x4229; // DEBUG413
  strcpy(labels[labelsc].nome, "GETUC"); labels[labelsc++].value = 0x0fe8; // DEBUG413
  strcpy(labels[labelsc].nome, "TOUPPER"); labels[labelsc++].value = 0x0fe9; // DEBUG413
  strcpy(labels[labelsc].nome, "UPCAS"); labels[labelsc++].value = 0x0fe9; // m100def
  strcpy(labels[labelsc].nome, "GETLNP"); labels[labelsc++].value = 0X4644; // DEBUG413
  strcpy(labels[labelsc].nome, "GETLN"); labels[labelsc++].value = 0X463e; // DEBUG413
  strcpy(labels[labelsc].nome, "CHKCHR"); labels[labelsc++].value = 0x5d46; // DEBUG413
  strcpy(labels[labelsc].nome, "INBUF"); labels[labelsc++].value = 0xf685; // DEBUG413
  strcpy(labels[labelsc].nome, "POSIT"); labels[labelsc++].value = 0X427C; // m100def
  // set current cursor location
  // Entry Conditions: H=column number (1-40), L=row number (1-8)
  // Exit Conditions: None
  strcpy(labels[labelsc].nome, "BARPOS"); labels[labelsc++].value = 0X59C9; // m100def
  // the Bar Cursor is the file cursor in MENU
  // position cursor on one of 24 bar positions
  // Entry Conditions: HL=position 0-23
  // Exit Conditions: none
  strcpy(labels[labelsc].nome, "BARCUR"); labels[labelsc++].value = 0X59E5; // m100def
  // toggle bar cursor to opposite state
  // Entry Conditions: HL=position 0-23
  // Exit Conditions: none
  strcpy(labels[labelsc].nome, "DTLINE"); labels[labelsc++].value = 0X5A18; // m100def
  // Print day/date/time as it appears on MENU screen
  // at current cursor location
  // updated time before printing it
  // Entry Conditions: None
  strcpy(labels[labelsc].nome, "TDDPT"); labels[labelsc++].value = 0X5A15; // m100def
  // print time on top line of screen, no cls
  // Entry Conditions: none
  // Exit  Conditions: all registers destroyed
  strcpy(labels[labelsc].nome, "PFRE"); labels[labelsc++].value = 0X7EAC; // m100def
  // print number of bytes of free memory
  // Entry Conditions: none
  // Exit  Conditions: none
  strcpy(labels[labelsc].nome, "ALTLCD"); labels[labelsc++].value = 0XFCC0; // m100def
  // start of alternate screen image 320 bytes
  strcpy(labels[labelsc].nome, "ALTLCDE"); labels[labelsc++].value = 0XFDFF; // m100def
  // end of alternate LCD screen image
  strcpy(labels[labelsc].nome, "KEYWTU"); labels[labelsc++].value = 0X5D64; // m100def
  // wait for key to be pressed, convert it to uppercase
  // Entry Conditions: None
  // Exit  Conditions: A=uppercase key pressed
  // function keys return preprogrammed strings
  strcpy(labels[labelsc].nome, "BRKCHK"); labels[labelsc++].value = 0X729F; // m100def
  // check to see if SHIFT-BREAK pressed
  // Entry Conditions: None
  // Exit  Conditions: Zero and Carry set on BREAK pressed
  // 		   A destroyed
  strcpy(labels[labelsc].nome, "SCANSP"); labels[labelsc++].value = 0X72B1; // m100def
  // scan keyboard column of BREAK,CAPS,NUM,CODE,GRAPH,CONTROL,
  // SHIFT key. Reset bits in A corresponding to the key pressed
  // Entry Conditions: None
  // Exit  Conditions: A=(BREAK,CAPS...)
  strcpy(labels[labelsc].nome, "DSPFNK"); labels[labelsc++].value = 0X42A8; // m100def
  // display function keys
  // Entry Conditions: none
  // Exit  Conditions: none
  strcpy(labels[labelsc].nome, "CSFKEY"); labels[labelsc++].value = 0X5B46; // m100def
  // cold start function key table address
  // 2 bytes
  strcpy(labels[labelsc].nome, "BASFKY"); labels[labelsc++].value = 0XF80A; // m100def
  // BASIC's function key table
  strcpy(labels[labelsc].nome, "BK2SK"); labels[labelsc++].value = 0X6C9C; // m100def
  // install BASIC fucntion key table
  // Entry Conditions: none
  // Exit Conditions: none
  strcpy(labels[labelsc].nome, "FKCTRL"); labels[labelsc++].value = 0XF650; // m100def
  // 1 byte
  // holds 0 or 80h
  // if 0, F-keys return string from system table
  // if 80, F-keys skip first 4 bytes and return what's left
  strcpy(labels[labelsc].nome, "LBENBL"); labels[labelsc++].value = 0XFAAD; // m100def
  // 1 byte
  // set to FFh to enable the label line
  strcpy(labels[labelsc].nome, "SNDCOM"); labels[labelsc++].value = 0X6E3A; // m100def
  // send a character to RS232-C or modem (without
  // XON/XOFF flow control)
  // Entry conditions: C = character to be sent
  // Exit conditions:  unknown
  strcpy(labels[labelsc].nome, "ENDDIR"); labels[labelsc++].value = 0XFF; // m100def
  // this value in byte 1 means no further entries
  strcpy(labels[labelsc].nome, "FDIR"); labels[labelsc++].value = 0XF9BA; // m100def
  // 2 bytes
  // points to first file in directory
  strcpy(labels[labelsc].nome, "PRSNAM"); labels[labelsc++].value = 0X4C0B; // m100def
  // parse a filename "file.do" into FILNAM
  // Entry Conditions: HL->file name
  //                   E=length of file name
  // Exit  Conditions: filename into FILNAM (fc93)
  strcpy(labels[labelsc].nome, "FNDFIL"); labels[labelsc++].value = 0X20AF; // m100def
  // search directory for file named at FC93h
  // Entry Conditions: FC93h+ contains filename
  // Exit Conditions: if Z clear the HL->directory
  //                     Z set, no file found
  strcpy(labels[labelsc].nome, "CHKFN"); labels[labelsc++].value = 0X5AAB; // m100def
  // search for a full-size filename
  // this is a workaround for the fact that chkdc can only
  // find 4 letter names like note.do and adrs.do
  // Entry Conditions: A=0ah
  //                   DE=address of null-terminated
  //                   filename
  // Exit Conditions:  HL=address of directory entry
  //                   Z flag=0 if file found
  //                   Z flag=1 if file not found
  strcpy(labels[labelsc].nome, "MKPNAM"); labels[labelsc++].value = 0X59AD; // m100def
  // convert dir entry style filename 'FILE  DO' to
  // 'FILE.DO'
  // Entry Conditions: DE -> directory name
  //                   HL -> output buffer
  // Exit Conditions: none
  strcpy(labels[labelsc].nome, "DIROK"); labels[labelsc++].value = 0X2146; // m100def
  // sort directory
  // Entry Conditions: none
  // Exit Conditions: none
  strcpy(labels[labelsc].nome, "NXTDIR"); labels[labelsc++].value = 0X20D5; // m100def
  // retrieve the next directory entry of an active file
  // Entry Conditions: HL should point to directory entry
  //    before the starting point.  search routine begins
  //    by pointing to the next entry
  // Exit  Conditions: Z set = no more entries
  //                   Z clear, HL -> dir. entry of an active file
  strcpy(labels[labelsc].nome, "FREDIR"); labels[labelsc++].value = 0X20EC; // m100def
  // locate an empty directory slot
  // Entry Conditions: none
  // Exit  Conditions: HL -> free slot
  strcpy(labels[labelsc].nome, "POPALL"); labels[labelsc++].value = 0X14ED; // m100def
  // pop all registers and return
  // Entry Conditions: registers were pushed in
  //       H,D,B,PSW order
  // Exit Conditions: none
  strcpy(labels[labelsc].nome, "BOOT"); labels[labelsc++].value = 0X7D33; // m100def
  // reboot.  rst 0 jumps here
  // Entry Conditions: none
  // Exit  Conditions: none
  strcpy(labels[labelsc].nome, "PWRRES"); labels[labelsc++].value = 0X143F; // m100def
  // POWER OFF, RESUME
  // Shuts the machine off. Program continues
  // when its turned back on.
  // Entry Conditions: None
  // Exit Conditions: None
  strcpy(labels[labelsc].nome, "BLKMV"); labels[labelsc++].value = 0X3469; // m100def
  // move B bytes from (DE) to M with increment
  // Entry Conditions: DE -> start of block to move
  //                   HL -> destination block
  //                   B = bytes to move
  // Exit  Conditions: HL=DE=HL+B+1
  //                   A, B destroyed
  strcpy(labels[labelsc].nome, "LINNUM"); labels[labelsc++].value = 0X08EC; // m100def
  // convert ASCII string to BASIC line number
  // ascii-to-integer, except that there's a range of
  // invalid BASIC line numbers that this won't return
  // Entry conditions: HL -> string to convert to int
  // Exit  Conditions: DE=integer
  strcpy(labels[labelsc].nome, "BASEXPR"); labels[labelsc++].value = 0X1297; // m100def
  // BASIC expression parser
  // for simple integer-digit conversion only.
  // Will dump you into BASIC with a ?SN ERROR on error...
  // use carefully
  // Entry Conditions: HL -> string to parse
  // Exit  Conditions: DE=result of expression
  strcpy(labels[labelsc].nome, "PRGADD"); labels[labelsc++].value = 0X344A; // m100def
  // return starting address of file
  // Entry Conditions: HL->dir entry
  // Exit  Conditions: DE=address
  strcpy(labels[labelsc].nome, "RSTDOG"); labels[labelsc++].value = 0X1BB1; // m100def
  // reset power-off watchdog
  // Entry Conditions: none
  // Exit  Conditions: none
  strcpy(labels[labelsc].nome, "TEXT"); labels[labelsc++].value = 0X5DEE; // m100def
  // clobbers non-file ram on entry
  strcpy(labels[labelsc].nome, "BASIC"); labels[labelsc++].value = 0X6C49; // m100def
  // clobbers non-file ram on entry
  strcpy(labels[labelsc].nome, "TELCOM"); labels[labelsc++].value = 0X5146; // m100def
  // clobbers non-file ram on entry
  strcpy(labels[labelsc].nome, "ADDRSS"); labels[labelsc++].value = 0X5B68; // m100def
  // clobbers non-file ram on entry
  strcpy(labels[labelsc].nome, "SCHEDL"); labels[labelsc++].value = 0X5B6F; // m100def
  // clobbers non-file ram on entry
  strcpy(labels[labelsc].nome, "SABASE"); labels[labelsc++].value = 0X5B74; // m100def
  // common database code for addrss/schedl
  // entry conditions: A=0 "Adrs" prompt
  //                   A!=0 "Schd" prompt
  //                   DE -> 8 byte null terminated filename
  //                     -or-
  //                   DE=FDD9h
  //                   FDD9 = full size null terminated
  //                          filename
  strcpy(labels[labelsc].nome, "ADSCFILE"); labels[labelsc++].value = 0XFFD9; // m100def
  // null-terminated filename buffer for addrss/schedl
  strcpy(labels[labelsc].nome, "TXTENT"); labels[labelsc++].value = 0X5F71; // m100def
  // entry into init of TEXT
  strcpy(labels[labelsc].nome, "FILOFF"); labels[labelsc++].value = 0XF6E7; // m100def
  // 2 bytes
  // offset from the beginning of the file being edited by
  // TEXT, so TEXT knows where to display

  // ;;I/O ports
  // I had no idea I/O ports were so screwy on the m100!
  // 70h-7fh free
  // 80h-8fh optional i/o controller
  // 90h-9fh optional answering modem

  // A0h-AFh bit 0: on/off for relay for signal selection
  //                of telephone unit
  //         bit 1: generate ENABLE signal for modem
  // b0h-bfh 8155 PIO
  // c0h-cfh enable data i/o on uart
  // d0h-dfh enable config on uart
  // e0h-efh enable signal for STROM, REMOTE
  //         also parallel port strobe and read keyboard
  // f0h-ffh enable signal for LCD
  strcpy(labels[labelsc].nome, "PHONEPORT"); labels[labelsc++].value = 0XA0; // m100def

  // b0=b8=command/status
  // b1=b9=port A
  // b2=ba=port B
  // b3=bb=port C
  // b4=bc=timer register low byte
  // b5=bd=timer register high byte
  strcpy(labels[labelsc].nome, "PIOCMD"); labels[labelsc++].value = 0XB0; // m100def
  strcpy(labels[labelsc].nome, "PIOA"); labels[labelsc++].value = 0XB1; // m100def
  strcpy(labels[labelsc].nome, "PIOB"); labels[labelsc++].value = 0XB2; // m100def
  strcpy(labels[labelsc].nome, "PIOC"); labels[labelsc++].value = 0XB3; // m100def
  strcpy(labels[labelsc].nome, "PIOTL"); labels[labelsc++].value = 0XB4; // m100def
  strcpy(labels[labelsc].nome, "PIOTH"); labels[labelsc++].value = 0XB5; // m100def
  strcpy(labels[labelsc].nome, "UARTDAT"); labels[labelsc++].value = 0XC0; // m100def
  strcpy(labels[labelsc].nome, "UARTCFG"); labels[labelsc++].value = 0XD0; // m100def
  strcpy(labels[labelsc].nome, "KEYPORT"); labels[labelsc++].value = 0XE0; // m100def
  strcpy(labels[labelsc].nome, "LCDPORT"); labels[labelsc++].value = 0XF0; // m100def
  strcpy(labels[labelsc].nome, "STKTOP"); labels[labelsc++].value = 0XFB9D; // m100def
  // 2 bytes
  // holds the beginning address of the stack
  strcpy(labels[labelsc].nome, "PTIMER"); labels[labelsc++].value = 0XF657; // m100def
  // 1 byte
  // Number of 6-second intervals before the m100
  // shuts itself off.
  // 0=disable power-off timer
  strcpy(labels[labelsc].nome, "POWERFLAG"); labels[labelsc++].value = 0XF932; // m100def
  // 1 byte
  // 0=do not shut off m100
  strcpy(labels[labelsc].nome, "PASTEPTR"); labels[labelsc++].value = 0XF88C; // m100def
  // 2 byte value
  // points to paste buffer
  // paste buffer is null-terminated
  strcpy(labels[labelsc].nome, "SHPRTPTR"); labels[labelsc++].value = 0XF88A; // m100def
  // 2 byte pointer
  // points to null-teriminated string returned by
  //    shft-print key
  // "llist\n" by default
  strcpy(labels[labelsc].nome, "BRKDIS"); labels[labelsc++].value = 0XF650; // m100def
  // 1 byte
  // store 80h here to disable the top row of keys (Fx,
  // print, arrows)
  // store 0h here to enable top row of keys
  strcpy(labels[labelsc].nome, "MAXRAM"); labels[labelsc++].value = 0XF5EF; // m100def
  // top of user ram, above this are system vars that
  // shouldn't be frobbed casually....62959 decimal
  strcpy(labels[labelsc].nome, "HIMEM"); labels[labelsc++].value = 0XF5F4; // m100def
  // 2 bytes -- bottom of protected memory for binaries
  strcpy(labels[labelsc].nome, "FREBAS"); labels[labelsc++].value = 0XF99A; // m100def
  // 2 bytes
  // points to start of free BASIC memory
  strcpy(labels[labelsc].nome, "MAXFILE"); labels[labelsc++].value = 0XFC82; // m100def
  // 1 byte
  // number of file buffers reserved
  strcpy(labels[labelsc].nome, "NEXTTXT"); labels[labelsc++].value = 0XF9A5; // m100def
  // 2 bytes
  // address of next text file
  strcpy(labels[labelsc].nome, "DOSTART"); labels[labelsc++].value = 0XFBAE; // m100def
  // 2 bytes
  // start of DO files
  strcpy(labels[labelsc].nome, "COSTART"); labels[labelsc++].value = 0XFBB0; // m100def
  // 2 bytes
  // start of CO files
  strcpy(labels[labelsc].nome, "VARSTART"); labels[labelsc++].value = 0XFBB2; // m100def
  // 2 bytes
  // start of BASIC variable space
  strcpy(labels[labelsc].nome, "ARRSTART"); labels[labelsc++].value = 0XFBB4; // m100def
  // 2 bytes
  // start of BASIC arrays
  strcpy(labels[labelsc].nome, "FRESTART"); labels[labelsc++].value = 0XFBB6; // m100def
  // 2 bytes
  // start of free memory
  strcpy(labels[labelsc].nome, "FILEEND"); labels[labelsc++].value = 0XF88C; // m100def
  // 2 bytes
  // end of file storage
  strcpy(labels[labelsc].nome, "SYSRAM"); labels[labelsc++].value = 0XFAC0; // m100def
  // 2 bytes
  // lowest ram used by system
  strcpy(labels[labelsc].nome, "HOOK"); labels[labelsc++].value = 0XFADA; // m100def
  // RST7 hook table address
  // you can call the Nth function in the table by:
  // rst7
  // db 2n
  // and RST7 will call HOOK+2n
  printf("done! There are %lu labels.\n", labelsc);

  char sline[256];
  memc = 0;
  unsigned short TOP = doCompile();
  printf("TOP is 0x%04x. ORG is 0x%04x\n", TOP, mem[0].addr);
  if (relocate == 'N') {
    printf("Relocate = [X], no need to recompile!\nDone...\n\n\n");
    return 1;
  }
  if(TOP == mem[0].addr && relocate != 'L') {
    printf("Sweet. TOP = ORG, no need to recompile!\nDone...\n\n\n");
    return 1;
  }
  if(relocate == 'L') {
    if(memc > 320) {
      unsigned int over = memc-320;
      printf(" /!\\ You asked for relocation to the LCD alternate buffer, but the code is too big by %d bytes. Giving up...\n", over);
      return 1;
    }
    TOP = 0xFE00; // BEGLCD
  }
  strcpy(fname1, fnamep);
  char tmp[12] = {0};
  sprintf(tmp, ".%04x.asm", TOP);
  strcat(fname1, tmp);
  printf("TOP & iaddr are different. Rewriting source to %s...\n", fname1);
  if (remove(fname3) == 0) {
    printf("File %s was deleted successfully.\n", fname3);
  } else {
    printf("File %s was NOT deleted successfully.\n", fname3);
  }
  if (remove(fname1) == 0) {
    printf("File %s was deleted successfully.\n", fname1);
  } else {
    printf("File %s was NOT deleted successfully.\n", fname1);
  }
  fclose(fin);
  printf("Opening file: %s\n", fname0);
  fin = fopen(fname0, "r");
  if (!fin) {
    printf("Error opening file: %s\n", fname0);
    return -1;
  }
  char done = 'N';
  fout1 = fopen(fname1, "w");
  if (!fout1) {
    printf("Error opening file: %s\n", fname1);
    return -1;
  }
  while (fgets(line, 256, fin)) {
    strcpy(sline, line);
    char *men;
    if (done == 'Y') {
      fprintf(fout1, "%s", line);
      // printf("%s", line);
    } else {
      unsigned int ix = 0;
      while (sline[ix] != ' ' && sline[ix] != '\t') ix += 1; // skip label
      while (sline[ix] == ' ' || sline[ix] == '\t') ix += 1; // skipe whitespace
      men = strtok(sline + ix, " \t:, \n");
      printf("line = %s; kwd = %s\n", sline, men);
      if (strcmp(men, "ORG") == 0) {
        printf("\tORG 0x%04X\n", TOP);
        fprintf(fout1, "\tORG 0x%04X\n", TOP);
        done = 'Y';
      } else {
        fprintf(fout1, "%s", line);
        // printf("%s", line);
      } 
    }
  }
  fclose(fout1);
  fclose(fin);
  printf("Done...\n");
  fin = fopen(fname1, "r");
  if (!fin) {
    printf(" ! Error opening file: %s\n", fname1);
    return -1;
  }
  printf(" * Compiling again...\n");
  memc = 0;
  TOP = doCompile();
  return 1;
}

unsigned short doCompile() {
  int i;
  char line[256];
  pass = 1;
  while (fgets(line, 256, fin)) {
    if (line[0] == ';') {
      // printf("%5i                           ==> %s", lc, line);
    } else {
      if (parse(line)) {
        // printf("%5i     %04X    ", lc, addr);
        // for(i=0;i<=addi;i++) printf("%02X ", prg[i]);
        // for(i=addi;i<3;i++) printf("   ");
        // printf("  ==> %s\n", line);
        addr += 1 + addi;
      } else {
        // if(strtok(line, "\n\r") != NULL) printf("%5i                           ==> %s\n", lc, line);
        // else printf("%5i                           ==> \n", lc);
      }
    }
    lc++;
  }
  rewind(fin);
  pass = 2;
  addr = 0;
  lc = 0;
  while (fgets(line, 256, fin)) {
    if (line[0] == ';') {
      fprintf(fout, "%5i                           ==> %s", lc, line);
    } else {
      if (parse(line)) {
        fprintf(fout, "%5i     %04X    ", lc, addr);
        for (i = 0; i <= addi; i++) {
          fprintf(fout, "%02X ", prg[i]);
          mem[memc].addr = addr + i;
          mem[memc].value = prg[i];
          memc++;
        }
        for (i = addi; i < 3; i++)fprintf(fout, "   ");
        fprintf(fout, "  ==> %s\n", line);
        addr += 1 + addi;
      } else {
        if (strtok(line, "\n\r") != NULL) fprintf(fout, "%5i                           ==> %s\n", lc, line);
        else fprintf(fout, "%5i                           ==> \n", lc);
      }
    }
    lc++;
  }
  fprintf(fout, "\n\nSYMBOLIC TABLE:\n\n");
  printf("   Saving SYMBOLIC TABLE.\n");
  for (i = 0; i < labelsc; i++) fprintf(fout, " %-10s  %04XH\n", labels[i].nome, labels[i].value);
  fprintf(fout, "\n\nMEM  (%i bytes):\n\n", memc);
  printf(" * RAM occupied:  (%i bytes):\n", memc);
  strcpy(fname2, fnamep);
  strcat(fname2, ".hex");
  printf(" * Saving %s, %s and %s:\n", fname2, fname3, fname4);
  fout2 = fopen(fname2, "w");
  if (!fout2) {
    printf("Error opening file: %s\n", fname2);
    return -1;
  }
  fout3 = fopen(fname3, "w");
  if (!fout3) {
    printf("Error opening file: %s\n", fname3);
    return -1;
  }
  fout4 = fopen(fname4, "w");
  if (!fout4) {
    printf("Error opening file: %s\n", fname4);
    return -1;
  }
  nb = 0;
  sum = 0;
  iaddr = mem[0].addr;
  fprintf(fout4, "%d,%d,%d", iaddr, (iaddr + memc - 1), iaddr);
  for (i = 0; i < memc; i++) {
    fprintf(fout3, "%c", mem[i].value);
    // printf(" %04XH  %02XH\n", mem[i].addr, mem[i].value);
    if (nb == 0) {
      iaddr = mem[i].addr;
      sprintf(values, "%02X", mem[i].value);
      fprintf(fout4, "\r\n");
    } else {
      sprintf(values, "%s%02X", values, mem[i].value);
    }
    fprintf(fout4, "%02X", mem[i].value);
    nb++;
    sum += mem[i].value;
    if ((mem[i + 1].addr != (mem[i].addr + 1)) || (nb == 16)) {
      sum += nb;
      sum += (iaddr & 0x00FF);
      sum += ((iaddr & 0xFF00) >> 8);
      // printf("sum=%02X %02X %02X\n", sum, ~sum, (~sum)+1);
      sum = (~sum) + 1;
      fprintf(fout2, ":%02X%04X00%s%02X\n", nb, iaddr, values, sum);
      fprintf(fout, ":%02X%04X00%s%02X\n", nb, iaddr, values, sum);
      nb = 0;
      sum = 0;
    }
  }
  fprintf(fout4, "%c", 0x1a);
  fprintf(fout2, ":00000001FF\n");
  fprintf(fout, ":00000001FF\n");
  fclose(fout);
  fclose(fout2);
  fclose(fout3);
  fin = fopen(fname3, "r");
  if (!fin) {
    printf("Error opening file: %s\n", fname3);
    return -1;
  }
  fseek(fin, 0L, SEEK_END);
  int sz = ftell(fin);
  fclose(fin);
  printf(" * File size: %d. ", sz);
  if (sz == memc) printf("This matches memc. Goodie.\n");
  else printf("This differs from memc (%d). No goodsky.\n", memc);
  unsigned int HIMEM = 62960 - sz - 1;
  printf("    CLEAR 256,%d\n", HIMEM);

  fclose(fout4);
  printf("%s\n\n\n", "All done!");
  return (HIMEM + 1);
}
