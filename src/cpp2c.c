#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *classToStruct(char *cpp) {
  char *ptr = cpp;

  while(ptr < (cpp+strlen(cpp))) {
    printf("%c", *ptr);
  }

  return NULL;
}
