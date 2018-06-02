#ifndef __ABORGAN_MEMBERFUNCTION__
#define __ABORGAN_MEMBERFUNCTION__

typedef struct Function {
  char *returnType;
  char *functionName;
  char *functionNewName;
  char **parameters;
  int numParameters;
  char *functionBody;

}Function;

Function *newFunction(char *myFunction);
char *getReturnType(char *myFunction);
char *getFunctionName(char *myFunction);
int getNumParameters(char *myFunction);
char **getParameters(char *myFunction, int numParameters);
char *adjustFunctionName(Function *myFunction);
char *getFunctionBody(char *myFunction);
void freeFunction(Function *myFunction);

#endif
