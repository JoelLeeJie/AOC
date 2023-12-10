#include <stdio.h>

long int* Add_Maps(FILE *readPtr, int *map_Size, int size, long int *seedArr, int size2);
long int RunMap(long int number, int map_Index, int *map_Size, long int *dynamicArr);
long int ConvertNumber(long int number, long int destination, long int source, long int range);
long int CheckMap(long int min, long int max, int *map_Size, long int *dynamicMaps);

typedef struct seedPair{
    int min, max;
} seedPair;