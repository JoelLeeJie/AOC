#include <stdio.h>

long int* Add_Maps(FILE *readPtr, int *map_Size, int size, long int *seedArr, int size2);
long int RunMap(long int number, int map_Index, int *map_Size, long int *dynamicArr);
long int ConvertNumber(long int number, long int destination, long int source, long int range);

/*
BRUTE FORCE METHOD <part 2>:
(note that the proper method is utilised in seedmap.c)
Work backwards, instead of converting seeds to results, convert the result into seed
Working in ascending order, find the smallest result which has a seed in the corresponding range.
*/