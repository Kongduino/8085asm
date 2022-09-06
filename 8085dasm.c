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
#include <stddef.h>
#include <ctype.h>
#include "8085asm.h"
#include "hex2label.h"

extern int read_ihx(const char * fname);
extern int mc;

int main(int argc, char** argv) {
  char fname[256];
  FILE* fin;
  int i, j, found;
  int dataAdress = 0;
  unsigned short paddr;
  int nameSet = 0;
  int b, e;
  size_t optind = 1;
  while (optind < argc && argv[optind][0] == '-') {
    // printf("arg: %s\n", argv[optind]);
    switch (argv[optind][1]) {
      case 'd':
        // printf("param: %s\n", argv[optind+1]);
        sscanf(argv[optind + 1], "%X", &dataAdress);
        printf("Data starts at 0x%04x\n", dataAdress);
        optind += 2;
        break;
      case 'x':
        // printf("param: %s\n", argv[optind+1]);
        sscanf(argv[optind + 1], "%X:%X", &b, &e);
        printf("Data from 0x%04x to 0x%04x\n", b, e);
        optind += 2;
        break;
      case 'f':
        // printf("param: %s\n", argv[optind+1]);
        strcpy(fname, argv[optind + 1]);
        printf("File name: %s\n", fname);
        nameSet = 1;
        optind += 2;
        break;
      default:
        fprintf(stderr, "Usage: %s [-s addr] [-f file...]\n", argv[0]);
        exit(-1);
    }
  }
  if (nameSet == 0) {
    printf("Enter the file name: ");
    scanf("%s", fname);
  }
  fin = fopen(fname, "r");
  if (!fin) {
    printf("Error open file:%s\n", fname);
    return -1;
  }
  printf(";LCGamboa 8085 disassembler 2008\n\n");
  read_ihx(fname);
  printf("         ORG %04X\n", mem[0].addr);
  paddr = mem[0].addr;
  for (i = 0; i < mc; i++) {
    //printf("%04X  %02X\n",mem[i].addr,mem[i].value);
    if (dataAdress > 0 && mem[i].addr >= dataAdress) {
      printf("L%04X:   DB 0x%02x [%c]\n", mem[i].addr, mem[i].value, mem[i].value < 32 ? '.' : (char)mem[i].value);
    } else {
      if (mem[i].addr != paddr) {
        printf("         ORG %04X\n", mem[i].addr);
        paddr = mem[i].addr;
      }
      j = 0;
      do {
        if (opcode[j].opc == mem[i].value) {
          printf("L%04X:   %s ", mem[i].addr, opcode[j].men);
          if (opcode[j].nargs == 1) {
            if (opcode[j].uargs == 0) printf("%s ", opcode[j].arg1);
            else printf("%s, ", opcode[j].arg1);
          }
          if (opcode[j].nargs == 2) {
            printf("%s, %s ", opcode[j].arg1, opcode[j].arg2);
          }
          if (opcode[j].uargs > 0) {
            if (opcode[j].uargs == 2) {
              char tmp[10];
              sprintf(tmp, "0x%02X%02X", mem[i + 2].value, mem[i + 1].value);
              unsigned int v = mem[i + 2].value * 256 + mem[i + 1].value;
              found = -1;
              for (int ix = 0; ix < numLabels; ix ++) {
                if (mylabels[ix].value == v) {
                  printf("%s [%s]", mylabels[ix].name, tmp);
                  found = 0;
                  ix = numLabels + 1;
                }
              }
              if (found == -1) printf("%s", tmp);
            } else {
              printf("0x%02X", mem[i + 1].value);
            }
          }
          printf("\n");
          i += opcode[j].uargs;
          paddr += opcode[j].uargs + 1;
          break;
        }
      }
      while (strcmp(opcode[j++].men, "ENDO") != 0);
    }
  }
  return 1;
}
