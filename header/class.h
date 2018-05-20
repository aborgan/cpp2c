#ifndef __ABORGAN_CLASS__
#define __ABORGAN_CLASS__

typedef struct Class {
  int classNameLength;
  char *classStart;
  char *classNameStart;
  char *classEnd;
  char *className;

  char *(*setClassName)(char *, int);
}Class;

Class *newClass(char *classStart);
char *setClassName(char *className, int classNameLength);
void freeClass(Class *myClass);
char *classToStruct(char *cpp);
char *replaceStruct(char *cpp, Class *myClass);

#endif
