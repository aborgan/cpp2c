#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileIO.h"

int main(int argc, char *argv[]) {
  char *cpp;
  char *inFileName;
  char *outFileName;

  if(argc < 2) {
    printf("Must enter an input file\n");
    return 1;
  } else if(argc > 3) {
    printf("./cpp2c <input_file> <output_file>\n");
    return 1;
  }

  if(argc == 3) {
    outFileName = argv[2];
  }
  inFileName = argv[1];

  if(!(cpp = parseFile(inFileName))) {
    return 2;
  }

  if(outFileName) {
    writeFile(outFileName, cpp);
  } else {
    writeFile("assets/out.c", cpp);
  }
  printf("FILE CONTENTS:\n%s", cpp);
  free(cpp);
  return 0;
}
