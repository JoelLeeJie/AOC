#include <stdio.h>
#include <stdlib.h>
int GetLineLength(FILE* readPtr);
int GetGearRatio(FILE *readPtr, int lineLength);
int GetNumber(FILE *readPtr, long int *gears, size_t sizeofArr);