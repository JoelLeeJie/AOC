#include <stdio.h>

typedef struct threadArgs{
    long int start, end, *seedArr, *dynamicArr;
    int size_seedArr, *map_Size;
}threadArgs;

long int* Add_Maps(FILE *readPtr, int *map_Size, int size, long int *seedArr, int size2);
long int RunMap(long int number, int map_Index, int *map_Size, long int *dynamicArr);
long int ConvertNumber(long int number, long int destination, long int source, long int range);
int CheckInSeedRange(long int number, long int *seedArr, int size);
long int ResultToSeed(long int result, int map_Index, int *map_Size, long int *dynamicArr);
long int FindLowestResult(long int start, long int end, long int *seedArr, int size_seedArr, int *map_Size, long int *dynamicArr);
void* ThreadFunction(void *args);


/*
BRUTE FORCE METHOD <part 2>:
(note that the proper method is utilised in seedmap.c)
Work backwards, instead of converting seeds to results, convert the result into seed
Working in ascending order, find the smallest result which has a seed in the corresponding range.
*/