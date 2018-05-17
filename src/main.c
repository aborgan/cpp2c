#include <stdio.h>

#include "fileIO.h"

int main(int argc, char *argv[]) {
  if(argc != 1) {
    printf("Must enter an input file");
    return 1;
  }
  parseFile();
  return 0;
}
