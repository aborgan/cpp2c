#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

char *parseFile(char argv[]) {
  FILE *fp;

  if((fp = fopen(argv, "r"))) {
    char c;
    char *string;
    int size;
    int index = 0;

    fseek(fp, 0, SEEK_END);
    if(!(size = ftell(fp))) {
      printf("error: empty file\n");
      return NULL;
    }
    rewind(fp);

    if(!(string = malloc(sizeof(char) * size))) {
      printf("error allocating memory\n");
      return NULL;
    }

    while((c = fgetc(fp)) != EOF) {
      string[index++] = (char)c;
    }
    string[index] = '\0';
    
    fclose(fp);
    return string;
  }
  printf("error: file %s not found\n", argv);
  return NULL;
}
