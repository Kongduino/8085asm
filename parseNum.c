#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <ctype.h>

int main(int argc, char** argv) {
  char buff[32];
  unsigned int address = 0;
  if (argc > 1) {
    for(unsigned int x = 1; x < argc; x++) {
      int n0 = 0, n1 = 0, n2 = 0;
      if (argv[x][0] == '0' && (argv[x][1] == 'x' || argv[x][1] == 'X'))
        n0 = 1;
      if (argv[x][strlen(argv[x])-1] == 'h' || argv[x][strlen(argv[x])-1] == 'H')
        n1 = 1;
      if (argv[x][0] == '0' && (argv[x][1] == 'b' || argv[x][1] == 'B'))
        n2 = 1;
      if(n0 == 1) {
        sscanf(argv[x], "%X", &address);
        printf("[n0] Address = 0x%04x\n", address);
      }
      if(n1 == 1 && n0 == 0) {
        argv[x][strlen(argv[x])-1] = 0;
        sscanf(argv[x], "%X", &address);
        printf("[n1] Address = 0x%04x\n", address);
      }
      if(n1 == 0 && n0 == 0 && n2 == 1) {
        unsigned int z, y = strlen(argv[x]);
        address = 0;
        for (z=2; z<y; z++) {
          if (argv[x][z] != '0' && argv[x][z] != '1') {
            printf("Bad format!");
            break;
          }
          address = (address << 1) | (argv[x][z] == '0' ? 0 : 1);
        }
        printf("[n2] Address = 0x%04x\n", address);
      }
      if(n1 == 0 && n0 == 0 && n2 == 0) {
        unsigned int z, y = strlen(argv[x]);
        address = 0;
        for (z=0; z<y; z++) {
          if (argv[x][z] < '0' || argv[x][z] > '9') {
            printf("Bad format!");
            break;
          }
          address = (address*10) + (argv[x][z]-'0');
        }
        printf("[dec] Address = 0x%04x\n", address);
      }
    }
  }
  return 1;
}
