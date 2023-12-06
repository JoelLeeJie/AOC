#include <stdio.h>
int GetLineLength(FILE* readPtr);
int CheckAdjacentSymbol(FILE *readPtr, int lineLength);
int isSymbol(char input);
int CheckSides(char *buffer, int lineLength, int linePos);
