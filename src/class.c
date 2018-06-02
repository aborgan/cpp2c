#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <cpp2c.h>
#include <class.h>

Class *newClass(char *classStart) {
  Class *newClass;
  char *tmpPtr = classStart;

  if(!classStart) {
    printf("newClass Error: classStart cannot be NULL\n");
    return NULL;
  }
  newClass = malloc(sizeof(Class));
  if(!newClass) {
    printf("newClass Error: malloc error trying to create class\n");
    return NULL;
  }
  newClass->classStart = NULL;
  newClass->classNameStart = NULL;
  newClass->classEnd = NULL;
  newClass->className = NULL;
  newClass->memberFunctions = NULL;
  newClass->classNameLength = 0;
  newClass->numMemberFunctions = 0;


  newClass->classStart = classStart;
  tmpPtr+=5;
  while(isspace(*tmpPtr)) {
    tmpPtr++;
  }
  newClass->classNameStart = tmpPtr;
  while(!isspace(*tmpPtr) && !(*tmpPtr == '{')) {
    tmpPtr++;
    newClass->classNameLength++;
  }

  newClass->className = setClassName(newClass->classNameStart, newClass->classNameLength);
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

  newClass->numMemberFunctions = getNumMemberFunctions(tmpPtr, newClass->classEnd);
  newClass->memberFunctions = getMemberFunctions(newClass);

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
  if(!myClass) { 
    return;
  }
  if(myClass->className) {
    free(myClass->className);
  }
  if(myClass->numMemberFunctions && myClass->memberFunctions) {
    for(int i = 0; i < myClass->numMemberFunctions; i++) {
      if(myClass->memberFunctions[i]) {
        freeMemberFunction(myClass->memberFunctions[i]);
      }
    }
  }
  if(myClass->memberFunctions) {
    free(myClass->memberFunctions);
  }
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


char *skipInnerClass(char *ptr) {
  char *innerClassEnd;

  if(isKeyWord(ptr, "class")) {
    while(*ptr != '{') {
      ptr++;
    }
    innerClassEnd = getClosingBracket(ptr);
    if(!innerClassEnd) {
      printf("skipInnerClass Error: can't find closing bracket\n");
      return NULL;
    }
    return innerClassEnd;
  }
  printf("skipInnerClass Error: input not pointing to keyword \"class\"\n");
  return NULL;
}


int getNumMemberFunctions(char *classStart, char *classEnd) {
  char *ptr = classStart;
  int numFunctions = 0;

  if(!classStart) {
    printf("getNumMemberFunctions error: classStart can't be NULL\n");
    return -1;
  } else if(!classEnd) {
    printf("getNumMemberFunctions Error: classEnd can't be NULL\n");
    return -1;
  } else if(classEnd < classStart) {
    printf("getNumMemberFunctions Error: classEnd can't be less than classStart\n");
    return -1;
  }

  while(ptr != classEnd) {
    if(isKeyWord(ptr, "class")) {
      ptr = skipInnerClass(ptr);
      if(!ptr) {
        printf("getNumMemberFunctions error: couldn't skip inner class\n");
        return -1;
      }
    }
    if(isFunction(ptr)) {
      numFunctions++;
    }
    ptr++;
  }
  return numFunctions;
}


MemberFunction **getMemberFunctions(Class *myClass) {
  MemberFunction **myMemFuncArr;
  int count = 0;
  char *ptr = myClass->classStart;

  if(myClass->numMemberFunctions < 0) {
    printf("getMemberFunctions Error: numMemberFunctions must be at least 0\n");
    return NULL;
  } else if(myClass->numMemberFunctions == 0) {
    return NULL;
  }
  if(!myClass->classStart) {
    printf("getMemberFunctions error: classStart can't be NULL\n");
    return NULL;
  } else if(!myClass->classEnd) {
    printf("getMemberFunctions Error: classEnd can't be NULL\n");
    return NULL;
  } else if(myClass->classEnd < myClass->classStart) {
    printf("getMemberFunctions Error: classEnd can't be less than classStart\n");
    return NULL;
  }

  myMemFuncArr = malloc(sizeof(MemberFunction *)*myClass->numMemberFunctions);
  if(!myMemFuncArr) { 
    printf("getMemberFunctions Error: couldn't allocate memory for myMemFuncArr\n");
    return NULL;
  }

  while(ptr != myClass->classEnd) {
    ptr++;
    if(isKeyWord(ptr, "class")) {
      ptr = skipInnerClass(ptr);
      if(!ptr) {
        printf("getMemberFunctions error: couldn't skip inner class\n");
	free(myMemFuncArr);
        return NULL;
      }
    }
    if(isFunction(ptr)) {
      myMemFuncArr[count] = newMemberFunction(ptr);
      if(!myMemFuncArr[count]) {
        printf("getMemberFunctions error: couldn't get new memberFunction\n");
	for(int i = 0; i < count; i++) {
	  freeMemberFunction(myMemFuncArr[i]);
	}
	free(myMemFuncArr);
	return NULL;
      }
      count++;
    }
  }

  return myMemFuncArr;
}
