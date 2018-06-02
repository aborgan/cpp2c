#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <class.h>
#include <function.h>
#include <cpp2c.h>

Function *newFunction(char *functionStart) {
  Function *newFunction;
  char *ptr = functionStart;

  if(!functionStart) {
    printf("newFunction Error: no returnType specified\n");
    return NULL;
  }

  newFunction = malloc(sizeof(Function));
  if(!newFunction) {
    printf("newFunction Error: malloc error for newFunction\n");
    return NULL;
  }
  newFunction->returnType = NULL;
  newFunction->functionName = NULL;
  newFunction->functionNewName = NULL;
  newFunction->parameters = NULL;
  newFunction->numParameters = 0;
  newFunction->functionBody = NULL;

  newFunction->returnType = getReturnType(ptr);
  if(!newFunction->returnType) {     
    printf("newFunction Error: error in return type\n");
    freeFunction(newFunction);
    return NULL;
  }
  printf("newFunction DEBUGGING: returnType: %s\n", newFunction->returnType);
 
  newFunction->functionName = getFunctionName(ptr);
  if(!newFunction->functionName) {
    printf("newFunction Error: error in getFunctionName\n");
    freeFunction(newFunction);
    return NULL;
  }
  printf("newFunction DEBUGGING: functionName: %s\n", newFunction->functionName);  

  newFunction->numParameters = getNumParameters(ptr);
  printf("newFunction DEBUGGING: numParams: %d\n", newFunction->numParameters);

  newFunction->parameters = getParameters(ptr, newFunction->numParameters);
  if(!newFunction->parameters) {
    printf("newFunction Error: error in getParameters\n");
    freeFunction(newFunction);
    return NULL;
  }
  for(int i = 0; i < newFunction->numParameters; i++) {
    printf("newFunction DEBUGGING: param %d: %s\n", i, newFunction->parameters[i]);
  }

  newFunction->functionBody = getFunctionBody(ptr);
  if(!newFunction->functionBody) {
    printf("newFunction Error: error in getFunctionBody\n");
    freeFunction(newFunction);
    return NULL;
  }
  printf("newFunction DEBUGGING: functionBody: %s\n", newFunction->functionBody);

  return newFunction;
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


char *adjustFunctionName(Function *myFunction) {
  char *adjustedName;
  if(!myFunction->functionName) {
    printf("adjustFunctionName Error: no name specified\n");
    return NULL;
  } else if(!myFunction->returnType) {
    printf("adjustFunctionName Error: no return type specified\n");
    return NULL;
  }

  adjustedName = malloc(1+strlen(myFunction->functionName)+myFunction->numParameters);
  if(!adjustedName) {
    printf("adjustFunctionName Error: malloc error for adjustedName\n");
    return NULL;
  }
  strncpy(adjustedName, myFunction->returnType, 1);
  strncat(adjustedName, myFunction->functionName, strlen(myFunction->functionName));
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

void freeFunction(Function *myFunction) {
  if(!myFunction) {
    printf("freeMemberFunction Error: null pointer\n");
    return;
  }

  if(myFunction->returnType) {
    free(myFunction->returnType);
  }

  if(myFunction->functionName) {
    free(myFunction->functionName);
  }

  if(myFunction->functionNewName) {
    free(myFunction->functionNewName);
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
