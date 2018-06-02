#ifndef __ABORGAN_CPP2C__
#define __ABORGAN_CPP2C__

int isKeyWord(char *cpp, char *word);
int isFunction(char *functionStart);
char *getClosingBracket(char *openingBracket);
char *skipComment(char *cpp);

#endif
