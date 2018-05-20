#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <cpp2c.h>
#include <memberFunction.h>
#include <class.h>

int isKeyWord(char *cpp, char *word) {
  if(!cpp||!word) {
    return 0;
  }
  return (!strncmp(word, cpp, strlen(word)) && isspace(*(cpp-1)) && isspace(*(cpp+strlen(word))));
}

char *getClosingBracket(char *openingBracket) {
  if(!openingBracket) {
    printf("getClosingBracket Error: openingBracket can't be NULL\n");
    return NULL;
  }
  int maxLength = strlen(openingBracket);
  char *closingBracket = openingBracket+1;
  int bracketCount = 1;
  int count = 1;

  while (bracketCount > 0 && count < maxLength) {
    if(*closingBracket == '{') {
      bracketCount++;
    } else if(*closingBracket == '}') {
      bracketCount--;
    }

    if (bracketCount == 0) {
      return closingBracket;
    }
    closingBracket++;
    count++;
  }
  return NULL;
}

char *skipComment(char *cpp) {
  if(*cpp == '/') {
    if(*(cpp+1) == '*') {
      while(!(*cpp == '\0' || (*cpp == '*' && *(cpp+1) == '/'))) {
        cpp++;
      }
    } else if(*(cpp+1) == '/') {
      while(!(*cpp == '\n' || *cpp == '\r' || *cpp == '\0')) {
        cpp++;
      }
    }
  }
  return cpp;
}
