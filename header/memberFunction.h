#ifndef __ABORGAN_MEMBERFUNCTION__
#define __ABORGAN_MEMBERFUNCTION__

typedef struct MemberFunction {
  char *returnType;
  char *memberFunctionName;
  char *memberFunctionNewName;
  char **parameters;
  int numParameters;
  char *functionBody;

}MemberFunction;

MemberFunction *newMemberFunction(char *myFunction);
char *getReturnType(char *myFunction);
char *getFunctionName(char *myFunction);
int getNumParameters(char *myFunction);
char **getParameters(char *myFunction, int numParameters);
char *adjustMemberFunctionName(MemberFunction *myFunction);
char *getFunctionBody(char *myFunction);
void freeMemberFunction(MemberFunction *myFunction);

#endif
