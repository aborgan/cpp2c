#ifndef __ABORGAN_CLASS__
#define __ABORGAN_CLASS__

#include <function.h>

typedef struct Class {
  int classNameLength;
  char *classStart;
  char *classNameStart;
  char *classEnd;
  char *className;
  int numMemberFunctions;
  Function **memberFunctions;

}Class;

Class *newClass(char *classStart);
char *setClassName(char *className, int classNameLength);
void freeClass(Class *myClass);
char *classToStruct(char *cpp);
char *replaceStruct(char *cpp, Class *myClass);
char *skipInnerClass(char *ptr);
int getNumMemberFunctions(char *classStart, char *classEnd);
Function **getMemberFunctions(Class *myClass);

#endif
