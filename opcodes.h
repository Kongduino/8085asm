/*
     Code below is based in file asm-id.c of GNUSim8085 - http://gnusim8085.sourceforge.net/

       Copyright (C) 2003  Sridhar Ratnakumar <srid@nearfar.org>

       This file is part of GNUSim8085.

       GNUSim8085 is free software; you can redistribute it and/or modify
       it under the terms of the GNU General Public License as published by
       the Free Software Foundation; either version 2 of the License, or
       (at your option) any later version.

       GNUSim8085 is distributed in the hope that it will be useful,
       but WITHOUT ANY WARRANTY; without even the implied warranty of
       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
       GNU General Public License for more details.

       You should have received a copy of the GNU General Public License
       along with GNUSim8085; if not, write to the Free Software
       Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


static Opcode opcode[] = {
  /*
     { 0x, "", 0, " ", " " },
  */
  {0xCE, "ACI", 0, " ", " ", 1, "ADC A,", "", ""},
  {0x8F, "ADC", 1, "A", " ", 0, "ADC A,", "", ""},
  {0x88, "ADC", 1, "B", " ", 0, "ADC A,", "", ""},
  {0x89, "ADC", 1, "C", " ", 0, "ADC A,", "", ""},
  {0x8A, "ADC", 1, "D", " ", 0, "ADC A,", "", ""},
  {0x8B, "ADC", 1, "E", " ", 0, "ADC A,", "", ""},
  {0x8C, "ADC", 1, "H", " ", 0, "ADC A,", "", ""},
  {0x8D, "ADC", 1, "L", " ", 0, "ADC A,", "", ""},
  {0x8E, "ADC", 1, "M", " ", 0, "ADC A,", "(HL)", ""},
  {0x87, "ADD", 1, "A", " ", 0, "ADD A,", "", ""},
  {0x80, "ADD", 1, "B", " ", 0, "ADD A,", "", ""},
  {0x81, "ADD", 1, "C", " ", 0, "ADD A,", "", ""},
  {0x82, "ADD", 1, "D", " ", 0, "ADD A,", "", ""},
  {0x83, "ADD", 1, "E", " ", 0, "ADD A,", "", ""},
  {0x84, "ADD", 1, "H", " ", 0, "ADD A,", "", ""},
  {0x85, "ADD", 1, "L", " ", 0, "ADD A,", "", ""},
  {0x86, "ADD", 1, "M", " ", 0, "ADD A,", "(HL)", ""},
  {0xC6, "ADI", 0, " ", " ", 1, "ADD A,", "", ""},
  {0xA7, "ANA", 1, "A", " ", 0, "AND", "", ""},
  {0xA0, "ANA", 1, "B", " ", 0, "AND", "", ""},
  {0xA1, "ANA", 1, "C", " ", 0, "AND", "", ""},
  {0xA2, "ANA", 1, "D", " ", 0, "AND", "", ""},
  {0xA3, "ANA", 1, "E", " ", 0, "AND", "", ""},
  {0xA4, "ANA", 1, "H", " ", 0, "AND", "", ""},
  {0xA5, "ANA", 1, "L", " ", 0, "AND", "", ""},
  {0xA6, "ANA", 1, "M", " ", 0, "AND", "(HL)", ""},
  {0xE6, "ANI", 0, "M", " ", 1, "AND", ""},
  {0xCD, "CALL", 0, "M", " ", 2, "CALL", "", ""},
  {0xDC, "CC", 0, "M", " ", 2, "CALL C,", "", ""},
  {0xFC, "CM", 0, "M", " ", 2, "CALL M,", "", ""},
  {0x2F, "CMA", 0, "M", " ", 0, "CPL", "", ""},
  {0x3F, "CMC", 0, "M", " ", 0, "CCF", "", ""},
  {0xBF, "CMP", 1, "A", " ", 0, "CP", "", ""},
  {0xB8, "CMP", 1, "B", " ", 0, "CP", "", ""},
  {0xB9, "CMP", 1, "C", " ", 0, "CP", "", ""},
  {0xBA, "CMP", 1, "D", " ", 0, "CP", "", ""},
  {0xBB, "CMP", 1, "E", " ", 0, "CP", "", ""},
  {0xBC, "CMP", 1, "H", " ", 0, "CP", "", ""},
  {0xBD, "CMP", 1, "L", " ", 0, "CP", "", ""},
  {0xBE, "CMP", 1, "M", " ", 0, "CP", "(HL)", ""},
  {0xD4, "CNC", 0, "M", " ", 2, "CALL NC,", "", ""},
  {0xC4, "CNZ", 0, "M", " ", 2, "CALL NZ,", "", ""},
  {0xF4, "CP", 0, "M", " ", 2, "CALL P,", "", ""},
  {0xEC, "CPE", 0, "M", " ", 2, "CALL PE,", "", ""},
  {0xFE, "CPI", 0, "M", " ", 1, "CP", "", ""},
  {0xE4, "CPO", 0, "M", " ", 2, "CALL PO,", "", ""},
  {0xCC, "CZ", 0, "M", " ", 2, "CALL Z,", "", ""},
  {0x27, "DAA", 0, "M", " ", 0, "DAA", "", ""},
  {0x09, "DAD", 1, "B", " ", 0, "ADD", "HL,BC", ""},
  {0x19, "DAD", 1, "D", " ", 0, "ADD", "HL,DE", ""},
  {0x29, "DAD", 1, "H", " ", 0, "ADD", "HL,HL", ""},
  {0x39, "DAD", 1, "SP", " ", 0, "ADD", "HL,SP", ""},
  {0x3D, "DCR", 1, "A", " ", 0, "DEC", "", ""},
  {0x05, "DCR", 1, "B", " ", 0, "DEC", "", ""},
  {0x0D, "DCR", 1, "C", " ", 0, "DEC", "", ""},
  {0x15, "DCR", 1, "D", " ", 0, "DEC", "", ""},
  {0x1D, "DCR", 1, "E", " ", 0, "DEC", "", ""},
  {0x25, "DCR", 1, "H", " ", 0, "DEC", "", ""},
  {0x2D, "DCR", 1, "L", " ", 0, "DEC", "", ""},
  {0x35, "DCR", 1, "M", " ", 0, "DEC", "(HL)", ""},
  {0x0B, "DCX", 1, "B", " ", 0, "DEC", "BC", ""},
  {0x1B, "DCX", 1, "D", " ", 0, "DEC", "DE", ""},
  {0x2B, "DCX", 1, "H", " ", 0, "DEC", "HL", ""},
  {0x3B, "DCX", 1, "SP", " ", 0, "DEC", "SP", ""},
  {0xF3, "DI", 0, " ", " ", 0, "DI", "", ""},
  {0xFB, "EI", 0, " ", " ", 0, "EI", "", ""},
  {0x76, "HLT", 0, " ", " ", 0, "HALT", "", ""},
  {0xDB, "IN", 0, " ", " ", 1, "IN A,", "#(%s)", ""},
  {0x3C, "INR", 1, "A", " ", 0, "INC", "", ""},
  {0x04, "INR", 1, "B", " ", 0, "INC", "", ""},
  {0x0C, "INR", 1, "C", " ", 0, "INC", "", ""},
  {0x14, "INR", 1, "D", " ", 0, "INC", "", ""},
  {0x1C, "INR", 1, "E", " ", 0, "INC", "", ""},
  {0x24, "INR", 1, "H", " ", 0, "INC", "", ""},
  {0x2C, "INR", 1, "L", " ", 0, "INC", "", ""},
  {0x34, "INR", 1, "M", " ", 0, "INC", "(HL)", ""},
  {0x03, "INX", 1, "B", " ", 0, "INC", "BC", ""},
  {0x13, "INX", 1, "D", " ", 0, "INC", "DE", ""},
  {0x23, "INX", 1, "H", " ", 0, "INC", "HL", ""},
  {0x33, "INX", 1, "SP", " ", 0, "INC", "SP", ""},
  {0xDA, "JC", 0, " ", " ", 2, "JP C,", "", ""},
  {0xFA, "JM", 0, " ", " ", 2, "JP M,", "", ""},
  {0xC3, "JMP", 0, " ", " ", 2, "JP", "", ""},
  {0xD2, "JNC", 0, " ", " ", 2, "JP NC,", "", ""},
  {0xC2, "JNZ", 0, " ", " ", 2, "JP NZ,", "", ""},
  {0xF2, "JP", 0, " ", " ", 2, "JP P,", "", ""},
  {0xEA, "JPE", 0, " ", " ", 2, "JP PE,", "", ""},
  {0xE2, "JPO", 0, " ", " ", 2, "JP PO,", "", ""},
  {0xCA, "JZ", 0, " ", " ", 2, "JP Z,", "", ""},
  {0x3A, "LDA", 0, " ", " ", 2, "LD A,", "#(%s)", ""},
  {0x0A, "LDAX", 1, "B", " ", 0, "LD A,", "(BC)", ""},
  {0x1A, "LDAX", 1, "D", " ", 0, "LD A,", "(DE)", ""},
  {0x2A, "LHLD", 0, " ", " ", 2, "LD HL,", "#(%s)", ""},
  {0x01, "LXI", 1, "B", " ", 2, "LD", "BC", ""},
  {0x11, "LXI", 1, "D", " ", 2, "LD", "DE", ""},
  {0x21, "LXI", 1, "H", " ", 2, "LD", "HL", ""},
  {0x31, "LXI", 1, "SP", " ", 2, "LD", "SP", ""},
  {0x7F, "MOV", 2, "A", "A", 0, "LD", "", ""},
  {0x78, "MOV", 2, "A", "B", 0, "LD", "", ""},
  {0x79, "MOV", 2, "A", "C", 0, "LD", "", ""},
  {0x7A, "MOV", 2, "A", "D", 0, "LD", "", ""},
  {0x7B, "MOV", 2, "A", "E", 0, "LD", "", ""},
  {0x7C, "MOV", 2, "A", "H", 0, "LD", "", ""},
  {0x7D, "MOV", 2, "A", "L", 0, "LD", "", ""},
  {0x7E, "MOV", 2, "A", "M", 0, "LD", "", "(HL)"},
  {0x47, "MOV", 2, "B", "A", 0, "LD", "", ""},
  {0x40, "MOV", 2, "B", "B", 0, "LD", "", ""},
  {0x41, "MOV", 2, "B", "C", 0, "LD", "", ""},
  {0x42, "MOV", 2, "B", "D", 0, "LD", "", ""},
  {0x43, "MOV", 2, "B", "E", 0, "LD", "", ""},
  {0x44, "MOV", 2, "B", "H", 0, "LD", "", ""},
  {0x45, "MOV", 2, "B", "L", 0, "LD", "", ""},
  {0x46, "MOV", 2, "B", "M", 0, "LD", "", "(HL)"},
  {0x4F, "MOV", 2, "C", "A", 0, "LD", "", ""},
  {0x48, "MOV", 2, "C", "B", 0, "LD", "", ""},
  {0x49, "MOV", 2, "C", "C", 0, "LD", "", ""},
  {0x4A, "MOV", 2, "C", "D", 0, "LD", "", ""},
  {0x4B, "MOV", 2, "C", "E", 0, "LD", "", ""},
  {0x4C, "MOV", 2, "C", "H", 0, "LD", "", ""},
  {0x4D, "MOV", 2, "C", "L", 0, "LD", "", ""},
  {0x4E, "MOV", 2, "C", "M", 0, "LD", "", "(HL)"},
  {0x57, "MOV", 2, "D", "A", 0, "LD", "", ""},
  {0x50, "MOV", 2, "D", "B", 0, "LD", "", ""},
  {0x51, "MOV", 2, "D", "C", 0, "LD", "", ""},
  {0x52, "MOV", 2, "D", "D", 0, "LD", "", ""},
  {0x53, "MOV", 2, "D", "E", 0, "LD", "", ""},
  {0x54, "MOV", 2, "D", "H", 0, "LD", "", ""},
  {0x55, "MOV", 2, "D", "L", 0, "LD", "", ""},
  {0x56, "MOV", 2, "D", "M", 0, "LD", "", "(HL)"},
  {0x5F, "MOV", 2, "E", "A", 0, "LD", "", ""},
  {0x58, "MOV", 2, "E", "B", 0, "LD", "", ""},
  {0x59, "MOV", 2, "E", "C", 0, "LD", "", ""},
  {0x5A, "MOV", 2, "E", "D", 0, "LD", "", ""},
  {0x5B, "MOV", 2, "E", "E", 0, "LD", "", ""},
  {0x5C, "MOV", 2, "E", "H", 0, "LD", "", ""},
  {0x5D, "MOV", 2, "E", "L", 0, "LD", "", ""},
  {0x5E, "MOV", 2, "E", "M", 0, "LD", "", "(HL)"},
  {0x67, "MOV", 2, "H", "A", 0, "LD", "", ""},
  {0x60, "MOV", 2, "H", "B", 0, "LD", "", ""},
  {0x61, "MOV", 2, "H", "C", 0, "LD", "", ""},
  {0x62, "MOV", 2, "H", "D", 0, "LD", "", ""},
  {0x63, "MOV", 2, "H", "E", 0, "LD", "", ""},
  {0x64, "MOV", 2, "H", "H", 0, "LD", "", ""},
  {0x65, "MOV", 2, "H", "L", 0, "LD", "", ""},
  {0x66, "MOV", 2, "H", "M", 0, "LD", "", "(HL)"},
  {0x6F, "MOV", 2, "L", "A", 0, "LD", "", ""},
  {0x68, "MOV", 2, "L", "B", 0, "LD", "", ""},
  {0x69, "MOV", 2, "L", "C", 0, "LD", "", ""},
  {0x6A, "MOV", 2, "L", "D", 0, "LD", "", ""},
  {0x6B, "MOV", 2, "L", "E", 0, "LD", "", ""},
  {0x6C, "MOV", 2, "L", "H", 0, "LD", "", ""},
  {0x6D, "MOV", 2, "L", "L", 0, "LD", "", ""},
  {0x6E, "MOV", 2, "L", "M", 0, "LD", "", "(HL)"},
  {0x77, "MOV", 2, "M", "A", 0, "LD", "(HL)", ""},
  {0x70, "MOV", 2, "M", "B", 0, "LD", "(HL)", ""},
  {0x71, "MOV", 2, "M", "C", 0, "LD", "(HL)", ""},
  {0x72, "MOV", 2, "M", "D", 0, "LD", "(HL)", ""},
  {0x73, "MOV", 2, "M", "E", 0, "LD", "(HL)", ""},
  {0x74, "MOV", 2, "M", "H", 0, "LD", "(HL)", ""},
  {0x75, "MOV", 2, "M", "L", 0, "LD", "(HL)", ""},
  {0x3E, "MVI", 1, "A", " ", 1, "LD", "", ""},
  {0x06, "MVI", 1, "B", " ", 1, "LD", "", ""},
  {0x0E, "MVI", 1, "C", " ", 1, "LD", "", ""},
  {0x16, "MVI", 1, "D", " ", 1, "LD", "", ""},
  {0x1E, "MVI", 1, "E", " ", 1, "LD", "", ""},
  {0x26, "MVI", 1, "H", " ", 1, "LD", "", ""},
  {0x2E, "MVI", 1, "L", " ", 1, "LD", "", ""},
  {0x36, "MVI", 1, "M", " ", 1, "LD", "(HL)", ""},
  {0x00, "NOP", 0, "M", " ", 0, "NOP", "", ""},
  {0xB7, "ORA", 1, "A", " ", 0, "OR", "", ""},
  {0xB0, "ORA", 1, "B", " ", 0, "OR", "", ""},
  {0xB1, "ORA", 1, "C", " ", 0, "OR", "", ""},
  {0xB2, "ORA", 1, "D", " ", 0, "OR", "", ""},
  {0xB3, "ORA", 1, "E", " ", 0, "OR", "", ""},
  {0xB4, "ORA", 1, "H", " ", 0, "OR", "", ""},
  {0xB5, "ORA", 1, "L", " ", 0, "OR", "", ""},
  {0xB6, "ORA", 1, "M", " ", 0, "OR", "(HL)", ""},
  {0xF6, "ORI", 0, "M", " ", 1, "OR", "", ""},
  {0xD3, "OUT", 0, "M", " ", 1, "OUT", "#(%s),A", ""},
  {0xE9, "PCHL", 0, "M", " ", 0, "JP (HL)", "", ""},
  {0xC0, "RNZ", 0, "B", " ", 0, "RET NZ", "", ""},
  {0xC1, "POP", 1, "B", " ", 0, "POP", "BC", ""},
  {0xD1, "POP", 1, "D", " ", 0, "POP", "DE", ""},
  {0xE1, "POP", 1, "H", " ", 0, "POP", "HL", ""},
  {0xF1, "POP", 1, "PSW", " ", 0, "POP", "AF", ""},
  {0xC5, "PUSH", 1, "B", " ", 0, "PUSH", "BC", ""},
  {0xD5, "PUSH", 1, "D", " ", 0, "PUSH", "DE", ""},
  {0xE5, "PUSH", 1, "H", " ", 0, "PUSH", "HL", ""},
  {0xF5, "PUSH", 1, "PSW", " ", 0, "PUSH", "AF", ""},
  {0x17, "RAL", 0, "M", " ", 0, "RLA", "", ""},
  {0x1F, "RAR", 0, "M", " ", 0, "RRA", "", ""},
  {0xD8, "RC", 0, "M", " ", 0, "RET C", "", ""},
  {0xC9, "RET", 0, "M", " ", 0, "RET", "", ""},
  {0x20, "RIM", 0, "M", " ", 0, "XXX", "", ""},
  {0x07, "RLC", 0, "M", " ", 0, "RLCA", "", ""},
  {0xF8, "RM", 0, "M", " ", 0, "RET M", "", ""},
  {0xD0, "RNC", 0, "M", " ", 0, "RET NC", "", ""},
  {0xF0, "RP", 0, "M", " ", 0, "RET P", "", ""},
  {0xE8, "RPE", 0, "M", " ", 0, "RET PE", "", ""},
  {0xE0, "RPO", 0, "M", " ", 0, "RET PO", "", ""},
  {0x0F, "RRC", 0, "M", " ", 0, "RRCA", "", ""},
  {0xC7, "RST", 1, "0", " ", 0, "RST", "00H", ""},
  {0xCF, "RST", 1, "1", " ", 0, "RST", "08H", ""},
  {0xD7, "RST", 1, "2", " ", 0, "RST", "10H", ""},
  {0xDF, "RST", 1, "3", " ", 0, "RST", "18H", ""},
  {0xE7, "RST", 1, "4", " ", 0, "RST", "20H", ""},
  {0xEF, "RST", 1, "5", " ", 0, "RST", "28H", ""},
  {0xF7, "RST", 1, "6", " ", 0, "RST", "30H", ""},
  {0xFF, "RST", 1, "7", " ", 0, "RST", "38H", ""},
  {0xC8, "RZ", 0, " ", " ", 0, "RET Z", "", ""},
  {0x9F, "SBB", 1, "A", " ", 0, "SBC A,", "", ""},
  {0x98, "SBB", 1, "B", " ", 0, "SBC A,", "", ""},
  {0x99, "SBB", 1, "C", " ", 0, "SBC A,", "", ""},
  {0x9A, "SBB", 1, "D", " ", 0, "SBC A,", "", ""},
  {0x9B, "SBB", 1, "E", " ", 0, "SBC A,", "", ""},
  {0x9C, "SBB", 1, "H", " ", 0, "SBC A,", "", ""},
  {0x9D, "SBB", 1, "L", " ", 0, "SBC A,", "", ""},
  {0x9E, "SBB", 1, "M", " ", 0, "SBC A,", "(HL)", ""},
  {0xDE, "SBI", 0, "M", " ", 1, "SBC A,", "", ""},
  {0x22, "SHLD", 0, "M", " ", 2, "LD", "#(%s), HL", ""},
  {0x30, "SIM", 0, "M", " ", 0, "XXX", "", ""},
  {0xF9, "SPHL", 0, "M", " ", 0, "LD SP,HL", "", ""},
  {0x32, "STA", 0, "M", " ", 2, "LD", "#(%s),A", ""},
  {0x02, "STAX", 1, "B", " ", 0, "LD (BC),", "A", ""},
  {0x12, "STAX", 1, "D", " ", 0, "LD (DE),", "A", ""},
  {0x37, "STC", 0, "M", " ", 0, "SCF", "", ""},
  {0x97, "SUB", 1, "A", " ", 0, "SUB", "", ""},
  {0x90, "SUB", 1, "B", " ", 0, "SUB", "", ""},
  {0x91, "SUB", 1, "C", " ", 0, "SUB", "", ""},
  {0x92, "SUB", 1, "D", " ", 0, "SUB", "", ""},
  {0x93, "SUB", 1, "E", " ", 0, "SUB", "", ""},
  {0x94, "SUB", 1, "H", " ", 0, "SUB", "", ""},
  {0x95, "SUB", 1, "L", " ", 0, "SUB", "", ""},
  {0x96, "SUB", 1, "M", " ", 0, "SUB", "(HL)", ""},
  {0xD6, "SUI", 0, "M", " ", 1, "SUB", "", ""},
  {0xEB, "XCHG", 0, "M", " ", 0, "EX DE,HL", "", ""},
  {0xAF, "XRA", 1, "A", " ", 0, "XOR", "", ""},
  {0xA8, "XRA", 1, "B", " ", 0, "XOR", "", ""},
  {0xA9, "XRA", 1, "C", " ", 0, "XOR", "", ""},
  {0xAA, "XRA", 1, "D", " ", 0, "XOR", "", ""},
  {0xAB, "XRA", 1, "E", " ", 0, "XOR", "", ""},
  {0xAC, "XRA", 1, "H", " ", 0, "XOR", "", ""},
  {0xAD, "XRA", 1, "L", " ", 0, "XOR", "", ""},
  {0xAE, "XRA", 1, "M", " ", 0, "XOR", "(HL)", ""},
  {0xEE, "XRI", 0, "M", " ", 1, "XOR", "", ""},
  {0xE3, "XTHL", 0, "M", " ", 0, "EX (SP),HL", "", ""},
  // Additions from 8085undef.doc
  {0x08, "DSUB", 0, " ", " ", 0, "DSUB", "", ""}, // HL=HL-BC
  {0x10, "ARHL", 0, " ", " ", 0, "ARHL", "", ""}, // HL=HL/2
  {0x18, "RDEL", 0, " ", " ", 0, "RDEL", "", ""}, // rotate DE left thru carry: ;E[0]=c, c=D[7]
  {0x28, "LDHI", 0, " ", " ", 1, "LDHI", "", ""}, // DE=HL+following (unsigned) byte
  {0x38, "LDSI", 0, " ", " ", 1, "LDSI", "", ""}, // DE=SP+following (unsigned) byte
  {0xCB, "RSTV", 0, " ", " ", 0, "RSTV", "", ""}, // call 40h if overflow flag set
  {0xD9, "SHLX", 0, " ", " ", 0, "SHLX", "", ""}, // (DE)=HL
  {0xED, "LHLX", 0, " ", " ", 0, "LHLX", "", ""}, // HL=(DE)
  {0xDD, "JNUI", 0, " ", " ", 2, "JP NU,", "", ""}, // jump <address> if flag bit 5 (underflow) clear. See below
  {0xFD, "JUI", 0, " ", " ", 2, "JP U,", "", ""}, // jump <address> if flag bit 5 (underflow) set. See below

  {0xFF, "ENDO", 0, " ", " ", 0, "", "", ""}, // End of table
};

// ;new flag bits in F:
// ;bit 76543210
// ;bit SZUHxPVC
// ;S=sign
// ;Z=zero
// ;U=underflow (opcode.doc calls this T for "true sign")
// ;H=half-carry
// ;x=unused
// ;P=Parity even
// ;V=oVerflow  (opcode.doc calls this O for Overflow)
// ;C=Carry
// http://www.club100.org/memfiles/index.php?&direction=0&order=&directory=Willard%20Goosey/Linux%20cross%20development

// https://162.250.75.198/files/LIBRARY_web8201/M100SIG/Lib-08-Tech-Programming/OPTEST.DOC
/*
    The only unspecified instruction left is RST V (CB), a software
    interrupt which causes program execution to branch to location 0040 if
    the V flag (two's complement overflow, bit 1) is set.  Since location
    0040 is in ROM, and the only thing there is a BASIC lookup table, this
    is a particularly useless instruction for Model 100 users.  Try it if
    you like, but even proper execution will only send your Model 100 into
    never-never land.
*/