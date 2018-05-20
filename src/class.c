#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <cpp2c.h>
#include <class.h>

Class *newClass(char *classStart) {
  Class *newClass;
  char *tmpPtr = classStart;

  newClass = malloc(sizeof(Class));
  if(!newClass) {
    printf("newClass Error: malloc error trying to create class\n");
    return NULL;
  }
  newClass->classStart = classStart;
  newClass->classNameLength = 0;
  newClass->setClassName = &setClassName;

  tmpPtr+=5;
  while(isspace(*tmpPtr)) {
    tmpPtr++;
  }
  newClass->classNameStart = tmpPtr;
  while(!isspace(*tmpPtr)) {
    tmpPtr++;
    newClass->classNameLength++;
  }

  newClass->className = newClass->setClassName(newClass->classNameStart, newClass->classNameLength);
  if(!newClass->className) {
    freeClass(newClass);
    printf("newClass Error: Can't Find className\n");
    return NULL;
  }

  while(*tmpPtr != '{') {
    tmpPtr++;
  }

  newClass->classEnd = getClosingBracket(tmpPtr);
  if(!newClass->classEnd) {
    freeClass(newClass);
    printf("newClass Error: Can't Find Closing Bracket\n");
    return NULL;
  }

  return newClass;
}


char *setClassName(char *className, int classNameLength) {
  char *myClassName;

  if(classNameLength < 0) {
    printf("setClassName Error: classNameLength must be at least 0\n");
    return NULL;
  }
  myClassName = malloc(classNameLength+1);
  if (!myClassName) {
    printf("setClassName Error: not enough memory for malloc\n");
    return NULL;
  }
  strncpy(myClassName, className, classNameLength);
  myClassName[classNameLength] = '\0';
  return myClassName;
}


void freeClass(Class *myClass) {
  free(myClass->className);
  free(myClass);
}


char *classToStruct(char *cpp) {
  char *ptr = cpp;

  while(*ptr != '\0') {
    ptr = skipComment(ptr);
    if(isKeyWord(ptr, "class")) {
      char *replacementStruct;
      char *tmpPtr = ptr;
      Class *myClass = newClass(tmpPtr);
      if(!myClass) {
        free(cpp);
	printf("classToStruct Error: newClass could not be created\n");
	exit(2);
      }

      if((replacementStruct = replaceStruct(cpp, myClass))) {
        cpp = replacementStruct;
      } else {
        printf("classToStruct Error: Error replacingStruct\n");
	free(cpp);
	freeClass(myClass);
	exit(4);
      }
      ptr = cpp;
      freeClass(myClass);
    }
    if(*ptr != '\0') {
      ptr++;
    }
  }
  return cpp;
}


char *replaceStruct(char *cpp, Class *myClass) {
  char *newCpp;
  int length = 0;
  if(!cpp) {
    printf("replaceStruct Error: cpp not Defined\n");
    return NULL;
  } else if(!myClass->classStart || !myClass->classNameStart || !myClass->classEnd || !myClass->className) {
    printf("replaceStruct Error: member in myClass not defined\n");
    if(!myClass->classStart) { printf("classStart not defined\n");}
    if(!myClass->classNameStart) { printf("classNameStart not defined\n");}
    if(!myClass->classEnd) { printf("classEnd not defined\n");}
    if(!myClass->className) { printf("className not defined\n");}
    return NULL;
  }

  newCpp = malloc(strlen(cpp)+9+myClass->classNameLength+1);
  if(!newCpp) {
    printf("replaceStruct Error: malloc memory error\n");
    return NULL;
  }

  strncpy(newCpp, cpp, myClass->classStart-cpp);
  length+=(myClass->classStart-cpp);
  newCpp[length] = '\0';
  strcat(newCpp, "typedef struct");
  length+=strlen("typedef struct");
  strncat(newCpp, myClass->classStart+5, myClass->classNameStart - (myClass->classStart+5));
  length+=(myClass->classNameStart-(myClass->classStart+5));
  newCpp[length] = '\0';
  strncat(newCpp, myClass->classNameStart, myClass->classEnd-myClass->classNameStart+1);
  length+=(myClass->classEnd-myClass->classNameStart+1);
  newCpp[length] = '\0';
  strncat(newCpp, myClass->className, myClass->classNameLength);
  length+=myClass->classNameLength;
  newCpp[length] = '\0';
  strncat(newCpp, myClass->classEnd+1, strlen(myClass->classEnd+1));
  length+=strlen(myClass->classEnd+1);
  newCpp[length] = '\0';

  free(cpp);
  return newCpp;
}
