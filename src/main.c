#include <stdio.h>

#include "fileIO.h"

int main(int argc, char *argv[]) {
  char *cpp;
  if(argc != 2) {
    printf("Must enter an input file\n");
    return 1;
  }
  if(!(cpp = parseFile(argv[1]))) {
    return 2;
  }
  printf("FILE CONTENTS: %s", cpp);
  return 0;
}
