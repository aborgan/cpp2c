#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

char *parseFile(char *fileName) {
  FILE *fp;

  if((fp = fopen(fileName, "r"))) {
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

    if(!(string = malloc(size+1))) {
      printf("error allocating memory\n");
      return NULL;
    }

    while((c = fgetc(fp)) != EOF) {
      string[index++] = (char)c;
    }
    string[index-1] = '\0';
    
    fclose(fp);
    return string;
  }
  printf("error: file %s not found\n", fileName);
  return NULL;
}

void writeFile(char *fileName, char *toWrite) {
  FILE *fp;
	
  if(!fileName) {
    printf("writeFile Error: No fileName given\n");
    return;
  }
  if(!toWrite) {
    printf("writeFile Error: No content to write\n");
    return;
  }
  
  if(!(fp = fopen(fileName, "w"))) {
    printf("Error opening file\n");
    return;
  }

  fprintf(fp, "%s", toWrite);
  fclose(fp);
  printf("file %s successfully written\n", fileName);
}
