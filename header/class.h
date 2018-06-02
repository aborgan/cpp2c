#ifndef __ABORGAN_CLASS__
#define __ABORGAN_CLASS__

#include <memberFunction.h>

typedef struct Class {
  int classNameLength;
  char *classStart;
  char *classNameStart;
  char *classEnd;
  char *className;
  int numMemberFunctions;
  MemberFunction **memberFunctions;

}Class;

Class *newClass(char *classStart);
char *setClassName(char *className, int classNameLength);
void freeClass(Class *myClass);
char *classToStruct(char *cpp);
char *replaceStruct(char *cpp, Class *myClass);
char *skipInnerClass(char *ptr);
int getNumMemberFunctions(char *classStart, char *classEnd);
MemberFunction **getMemberFunctions(Class *myClass);

#endif
