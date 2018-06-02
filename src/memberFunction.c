#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <class.h>
#include <memberFunction.h>
#include <cpp2c.h>

MemberFunction *newMemberFunction(char *functionStart) {
  MemberFunction *newMemberFunction;
  char *ptr = functionStart;

  if(!functionStart) {
    printf("newMemberFunction Error: no returnType specified\n");
    return NULL;
  }

  newMemberFunction = malloc(sizeof(MemberFunction));
  if(!newMemberFunction) {
    printf("newMemberFunction Error: malloc error for newMemberFunction\n");
    return NULL;
  }
  newMemberFunction->returnType = NULL;
  newMemberFunction->memberFunctionName = NULL;
  newMemberFunction->memberFunctionNewName = NULL;
  newMemberFunction->parameters = NULL;
  newMemberFunction->numParameters = 0;
  newMemberFunction->functionBody = NULL;

  newMemberFunction->returnType = getReturnType(ptr);
  if(!newMemberFunction->returnType) {     
    printf("newMemberFunction Error: error in return type\n");
    freeMemberFunction(newMemberFunction);
    return NULL;
  }
  printf("newMemberFunction DEBUGGING: returnType: %s\n", newMemberFunction->returnType);
 
  newMemberFunction->memberFunctionName = getFunctionName(ptr);
  if(!newMemberFunction->memberFunctionName) {
    printf("newMemberFunction Error: error in getFunctionName\n");
    freeMemberFunction(newMemberFunction);
    return NULL;
  }
  printf("newMemberFunction DEBUGGING: functionName: %s\n", newMemberFunction->memberFunctionName);  

  newMemberFunction->numParameters = getNumParameters(ptr);
  printf("newMemberFunction DEBUGGING: numParams: %d\n", newMemberFunction->numParameters);

  newMemberFunction->parameters = getParameters(ptr, newMemberFunction->numParameters);
  if(!newMemberFunction->parameters) {
    printf("newMemberFunction Error: error in getParameters\n");
    freeMemberFunction(newMemberFunction);
    return NULL;
  }
  for(int i = 0; i < newMemberFunction->numParameters; i++) {
    printf("newMemberFunction DEBUGGING: param %d: %s\n", i, newMemberFunction->parameters[i]);
  }

  newMemberFunction->functionBody = getFunctionBody(ptr);
  if(!newMemberFunction->functionBody) {
    printf("newMemberFunction Error: error in getFunctionBody\n");
    freeMemberFunction(newMemberFunction);
    return NULL;
  }
  printf("newMemberFunction DEBUGGING: functionBody: %s\n", newMemberFunction->functionBody);

  return newMemberFunction;
}


char *getReturnType(char *myFunction) {
  int count = 0;
  char *ptr = myFunction;
  char *newReturnType;
  int isPtr = 0;

  if(!myFunction) {
    printf("getReturnType Error: input to function is NULL\n");
    return NULL;
  }
 
  while(!isspace(*ptr) && *ptr != '*' && *ptr != '\0') {
    ptr++;
    count++;
  }

  if(isspace(*ptr)) {
    while(isspace(*ptr)) {
      ptr++;
    }
    if(*ptr == '*') {
      isPtr++;
      count+=2;
    }
  } else if(*ptr == '*') {
    isPtr++;
    count+=2;
  }

  newReturnType = malloc(count+1);
  if(!newReturnType) {
    printf("getReturnType Error: couldn't malloc for return type\n");
    return NULL;
  }

  if(isPtr) {
    strncpy(newReturnType, myFunction, count-2);
    strcat(newReturnType, " *");
  } else {
    strncpy(newReturnType, myFunction, count);
  }

  newReturnType[count] = '\0';

  return newReturnType;
}

char *getFunctionName(char *myFunction) {
  char *ptr = myFunction;
  int count = 0;
  char *functionNameStart;
  char *functionName;

  if(!myFunction) {
    printf("getFunctionName Error: function not defined\n");
    return NULL;
  }

  while(!isspace(*ptr) && *ptr != '*') {
    ptr++;
    if(*ptr == '\0') {
      printf("getFunctionName Error: unusual function declaration\n");
      return NULL;
    }
  }

  while(isspace(*ptr)) {
    ptr++;
    if(*ptr == '\0') {
      printf("getFunctionName Error: unusual function declaration\n");
      return NULL;
    }
  }

  if((*ptr) == '*') {
    ptr++;
  }

  while(isspace(*ptr)) {
    ptr++;
    if(*ptr == '\0') {
      printf("getFunctionName Error: unusual function declaration\n");
      return NULL;
    }
  }

  functionNameStart = ptr;
  while((*ptr) != '(' && !isspace(*ptr)) { 
    if(*ptr == '\0') {
      printf("getFunctionName Error: unusual function declaration\n");
      return NULL;
    }
    ptr++;
    count++;
  }

  functionName = malloc(count+1);
  if(!functionName) {
    printf("getFunctionName Error: couldn't malloc\n");
    return NULL;
  }

  strncpy(functionName, functionNameStart, count);
  functionName[count] = '\0';

  return functionName;
}


int getNumParameters(char *myFunction) {
  char *ptr = myFunction;
  int count = 0;

  if(!ptr) {
    printf("getNumParameters Error: Function not defined\n");
    return 0;
  }

  while(*ptr != '(') {
    if(*ptr == '\0') {
      printf("getNumParameters Error: unusual function declaration\n");
      return 0;
    }
    ptr++;
  }

  while(*ptr != ')') {
    if(*ptr == '\0') {
      printf("getNumParameters Error: unusual function declaration\n");
      return 0;
    } else if(*ptr == ',') {
      count++;
    }
    ptr++;
  }

  if(count > 0) {
    return count+1;
  }

  ptr--;
  while(*ptr != '(') {
    if(!isspace(*ptr)) {
      return 1;
    }
    ptr--;
  }

  return 0;
}


char **getParameters(char *myFunction, int numParameters) {
  char *ptr = myFunction;
  char *paramStart;
  char **parameters;
  int count = 0;
  int paramSize = 0;

  if(!myFunction) {
    printf("getParameters Error: myFunction not defined\n");
    return NULL;
  } else if(numParameters < 0) {
    printf("getParameters Error: numParameters can't be less than 0\n");
    return NULL;
  } else if(numParameters == 0) {
      parameters = malloc(sizeof(char*));
      if(!parameters) {
        printf("getParameters Error: malloc error making empty string\n");
	return NULL;
      }
      return parameters;
  }

  while(*ptr != '(') {
    if(*ptr == '\0') {
      printf("getParameters Error: unusual function definition\n");
      return NULL;
    }
    ptr++;
  }

  parameters = malloc(sizeof(char *) * numParameters);
  if(!parameters) {
    printf("getParameters Error: couldn't malloc for parameters\n");
    return NULL;
  }

  while(*ptr != ')') {
    ptr++;
    if(*ptr == '\0') {
      printf("getParameters Error: unusual function definition\n");
      for(int i = 0; i < count; i++) {
        free(parameters[i]);
      }
      free(parameters);
      return NULL;
    }
    paramStart = ptr;
    while(*ptr != ',' && *ptr != ')') {
      ptr++;
      paramSize++;
    }
    parameters[count] = malloc(paramSize+1);
    if(!parameters[count] || count+1 > numParameters) {
      if(!parameters[count]) {
        printf("getParameters Error: couldn't malloc for specific parameters\n"); 
      } else {
        printf("getParameters Error: more parameters found than numParameters\n");
      }

      for(int i = 0; i < count; i++) {
        free(parameters[i]);
      }
      free(parameters);
      return NULL;
    }
    strncpy(parameters[count], paramStart, paramSize);
    parameters[count][paramSize] = '\0';
    count++;
    paramSize = 0;
  }  
  
  return parameters;
}


char *adjustMemberFunctionName(MemberFunction *myFunction) {
  char *adjustedName;
  if(!myFunction->memberFunctionName) {
    printf("adjustMemberFunctionName Error: no name specified\n");
    return NULL;
  } else if(!myFunction->returnType) {
    printf("adjustMemberFunctionName Error: no return type specified\n");
    return NULL;
  }

  adjustedName = malloc(1+strlen(myFunction->memberFunctionName)+myFunction->numParameters);
  if(!adjustedName) {
    printf("adjustMemberFunctionName Error: malloc error for adjustedName\n");
    return NULL;
  }
  strncpy(adjustedName, myFunction->returnType, 1);
  strncat(adjustedName, myFunction->memberFunctionName, strlen(myFunction->memberFunctionName));
  for(int i = 0; i < myFunction->numParameters; i++) {
    strncat(adjustedName, myFunction->parameters[i], 1);
  }
  return adjustedName;
}


char *getFunctionBody(char *myFunction) {
  char *ptr = myFunction;
  char *functionBody;
  char *functionEnd;

  if(!myFunction) {
    printf("getFunctionBody Error: myFunction not defined\n");
    return NULL;
  }

  while(*ptr != '{') {
    if(*ptr == '\0') {
      printf("getFunctionBody Error: unusual function definiton\n");
      return NULL;
    }
    ptr++;
  }

  functionEnd = getClosingBracket(ptr);
  if(!functionEnd) {
    printf("getFunctionBody Error: Could not find closing bracket\n");
    return NULL;
  }

  functionBody = malloc(2+functionEnd-ptr);
  if(!functionBody) {
    printf("getFunctionBody Error: could not malloc\n");
    return NULL;
  }

  strncpy(functionBody, ptr, 1+functionEnd-ptr);
  functionBody[1+functionEnd-ptr] = '\0';
  return functionBody;
}

void freeMemberFunction(MemberFunction *myFunction) {
  if(!myFunction) {
    printf("freeMemberFunction Error: null pointer\n");
    return;
  }

  if(myFunction->returnType) {
    free(myFunction->returnType);
  }

  if(myFunction->memberFunctionName) {
    free(myFunction->memberFunctionName);
  }

  if(myFunction->memberFunctionNewName) {
    free(myFunction->memberFunctionNewName);
  }

  if(myFunction->parameters) {
    for(int i = 0; i < myFunction->numParameters; i++) {
      if(myFunction->parameters[i]) {
        free(myFunction->parameters[i]);
      }
    }
    free(myFunction->parameters);
  }

  if(myFunction->functionBody) {
    free(myFunction->functionBody);
  }

  free(myFunction);
}
