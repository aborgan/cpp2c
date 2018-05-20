#ifndef __ABORGAN_MEMBERFUNCTION__
#define __ABORGAN_MEMBERFUNCTION__

typedef struct MemberFunction {
  char *returnType;
  char *memberFunctionName;
  int memberFunctionNameLength;
  char *memberFunctionNewName;
  char **parameters;
  char *parameterStart;
  char *openingBracket;
  char *closingBracket;

  int (*getNumParameters)(char *parameterStart);
  char *(*adjustMemberFunctionName)(char *memberFunctionName);
}MemberFunction;

MemberFunction *newMemberFunction();
void freeMemberFunction();
int getNumParameters(char *parameterStart);
char *adjustMemberFunctionName(char *memberFunctionName);

#endif
