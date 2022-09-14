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
int labelsc = 0;
int pass;
int memc;

int parseNumber(char *arg) {
  int n0 = 0, n1 = 0, n2 = 0, myValue = 0;
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
      // printf("Bad format!");
      return -1;
    }
    myValue = (myValue * 10) + (arg[z] - '0');
  }
  // printf("[dec] myValue = 0x%04x\n", myValue);
  return myValue;
}

int parsearg(char* arg, char * line) {
  int i;
  if (arg == NULL) {
    printf("line %i error!!!!\n%s\n", lc + 1, line);
    exit(-1);
  };
  for (i = 0; i < labelsc; i++) {
    if (strcmp(labels[i].nome, arg) == 0) {
      return labels[i].value;
    };
  }
  // sscanf(arg, "%X", &i);
  // return i;
  return parseNumber(arg);
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
  }
  while (strcmp(opcode[i++].men, "ENDO") != 0);
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
      prg[addi+nx] = tmp[nx];
    }
    addi += (ny-1);
  }
  return 1;
}

int main(int argc, char** argv) {
  char fname[256];
  char fname2[256];
  char fname3[256];
  char fname4[256];
  FILE* fin;
  char line[256];
  int i;
  unsigned char sum;
  unsigned char nb;
  unsigned short iaddr;
  char values[100];
  char *fnamep;
  FILE *fout;
  FILE *fout2;
  FILE *fout3;
  FILE *fout4;
  if (argc == 2) {
    strcpy(fname, argv[1]);
  } else {
    printf("enter the file name: ");
    scanf("%s", fname);
    getchar();
  }
  fin = fopen(fname, "r");
  if (!fin) {
    printf("Error opening file: %s\n", fname);
    return -1;
  }
  fnamep = strtok(fname, ".");
  printf("LCGamboa 8085 assembler 2008\n\n");
  strcpy(fname2, fnamep);
  strcat(fname2, ".map");
  strcpy(fname3, fnamep);
  strcat(fname3, ".co");
  strcpy(fname4, fnamep);
  strcat(fname4, ".do");
  printf("* Saving %s:\n", fname2);
  fout = fopen(fname2, "w");
  if (!fout) {
    printf("Error opening file: %s\n", fname2);
    return -1;
  }
  fprintf(fout, "8085 Simple Assembler 2022\n\n");
  strcpy(labels[labelsc].nome, "BAUDST"); labels[labelsc++].value = 0x6E75;
  strcpy(labels[labelsc].nome, "BEGLCD"); labels[labelsc++].value = 0xFE00;
  strcpy(labels[labelsc].nome, "BRKCHK"); labels[labelsc++].value = 0x7283;
  strcpy(labels[labelsc].nome, "CARDET"); labels[labelsc++].value = 0x6EEF;
  strcpy(labels[labelsc].nome, "CASIN"); labels[labelsc++].value = 0x14B0;
  strcpy(labels[labelsc].nome, "CHGET"); labels[labelsc++].value = 0x12CB;
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
  strcpy(labels[labelsc].nome, "ENDLCD"); labels[labelsc++].value = 0xFF40;
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
  strcpy(labels[labelsc].nome, "POSIT"); labels[labelsc++].value = 0x4D7C;
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
  strcpy(labels[labelsc].nome, "SETSYS"); labels[labelsc++].value = 0x4235;
  strcpy(labels[labelsc].nome, "STAT"); labels[labelsc++].value = 0xF65B;
  strcpy(labels[labelsc].nome, "STFNK"); labels[labelsc++].value = 0x5A7C;
  strcpy(labels[labelsc].nome, "SYNCR"); labels[labelsc++].value = 0x6F85;
  strcpy(labels[labelsc].nome, "SYNCW"); labels[labelsc++].value = 0x6F46;
  strcpy(labels[labelsc].nome, "TIME"); labels[labelsc++].value = 0x190F;
  strcpy(labels[labelsc].nome, "UNLOCK"); labels[labelsc++].value = 0x4244;
  strcpy(labels[labelsc].nome, "UNPLOT"); labels[labelsc++].value = 0x744C;
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
  printf("* RAM occupied:  (%i bytes):\n", memc);
  strcpy(fname2, fnamep);
  strcat(fname2, ".hex");
  printf("* Saving %s, %s and %s:\n", fname2, fname3, fname4);
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
  fprintf(fout4, "10 AD = %d\n", iaddr);
  int lineNum = 11;
  fprintf(fout4, "%d N = %d\n", lineNum++, memc);
  fprintf(fout4, "%d FOR I = 1 TO N\n", lineNum++);
  fprintf(fout4, "%d READ X\n", lineNum++);
  fprintf(fout4, "%d POKE AD,X\n", lineNum++);
  fprintf(fout4, "%d AD=AD+1\n", lineNum++);
  fprintf(fout4, "%d NEXT I\n", lineNum++);
  fprintf(fout4, "%d CALL %d\n", lineNum++, iaddr);
  fprintf(fout4, "%d END\n", lineNum++);
  for (i = 0; i < memc; i++) {
    fprintf(fout3, "%c", mem[i].value);
    // printf(" %04XH  %02XH\n", mem[i].addr, mem[i].value);
    if (nb == 0) {
      iaddr = mem[i].addr;
      sprintf(values, "%02X", mem[i].value);
      fprintf(fout4, "\n%d DATA ", lineNum++);
    } else {
      fprintf(fout4, "%c", ',');
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
  fprintf(fout4, "%c", 0x0a);
  fprintf(fout4, "%c", 0x1a);
  fprintf(fout2, ":00000001FF\n");
  fprintf(fout, ":00000001FF\n");
  fclose(fout);
  fclose(fout2);
  fclose(fout3);
  fclose(fout4);
  printf("%s\n\n\n", "All done!");
  return 1;
}
