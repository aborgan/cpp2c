#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <cpp2c.h>
#include <class.h>
#include <memberFunction.h>

MemberFunction newMemberFunction(char *returnType) {
  MemberFunction myMemberFunction;
  myMemberFunction.getNumParameters = &getNumParameters;
  myMemberFunction.adjustMemberFunctionName = &adjustMemberFunctionName;
  return myMemberFunction;
}


int getNumParameters(char *parameterStart){
  return 0;
}


char *adjustMemberFunctionName(char *memberFunctionName) {
  return NULL;
}
