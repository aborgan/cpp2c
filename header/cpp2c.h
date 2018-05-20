#ifndef __ABORGAN_CPP2C__
#define __ABORGAN_CPP2C__

typedef struct Class{
  int classNameLength;
  char *classStart;
  char *classNameStart;
  char *classEnd;
  char *className;

  char *(*setClassName)(char *, int);
}Class;

Class newClass(char *classStart);
char *setClassName(char *className, int classNameLength);
void freeClass(Class myClass);


int isKeyWord(char *cpp, char *word);
char *classToStruct(char *cpp);
char *getClosingBracket(char *openingBracket);
char *replaceStruct(char *cpp, Class myClass);

#endif
