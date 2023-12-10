#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "seedmap.h"
#define num_Maps 10
#define num_Seeds 50

int main(void)
{
    //assumptions
    //1. Every line has 3 elements(ok).
    //2. These elements are in the format (Destination, Start, Range)
    int map_Size[num_Maps]; //leave space for 10 maps, but only using 7.
    FILE* readFile = fopen("input.txt", "r");
    if(readFile == NULL)
    {
        printf("Unable to read input file.");
        return 0;
    }

    long int seedArr[num_Seeds];
    memset(seedArr, -1, num_Seeds*8); 

    //returns a long int* dynamic array, with 3 elements per row. Number of rows per map is stored in map_Size.
    long int* dynamicMaps = Add_Maps(readFile, map_Size, num_Maps, seedArr, num_Seeds);
    

    //loop through each seed.
    long int lowest = __LONG_MAX__, result = 0;
    for(int i = 0; i<num_Seeds; i++)
    {
        if(seedArr[i] == -1) continue;
        result = seedArr[i];
        //Run each map one at a time for this seed.
        for(int currentMap = 0; currentMap<num_Maps; currentMap++)
        {
            if(map_Size[currentMap] == 0) continue;
            result = RunMap(result, currentMap, map_Size, dynamicMaps);
        }
        lowest = (result<lowest)? result : lowest;
        printf("%ld -- %ld\n", result, lowest);

    }   
    printf("\n%ld", lowest);


    /*
    //Proof of Concept. Print out the entire dynamic array, separate by map.
    int currentIndex = 0;
    for(size_t i = 0; i<sizeof(map_Size)/sizeof(map_Size[0]); i++) //iterate through each map.
    {
        if(map_Size[i] == 0) continue;
        printf("Map: %zu\n", i+1);
        for(int j = 0; j<map_Size[i]; j++) //iterate through each row in that map.
        {
            printf("%ld, %ld, %ld\n", dynamicMaps[currentIndex], dynamicMaps[currentIndex+1], dynamicMaps[currentIndex+2]);
            currentIndex +=3; 
        }
        printf("\n");
    }
    */

   
    fclose(readFile);
    free(dynamicMaps);
    
    return 0;
}

//Twilight's Seal of Approval:D
long int* Add_Maps(FILE *readPtr, int *map_Size, int size, long int *seedArr, int size2)
{
    //Look for semi:colon, then add until blank line(only have newline) is reached.
    char buffer[500];
    memset(map_Size, 0, size*4);
    int isMap = 0, index = -1;
    int counter = 0, currentArrSize = 0;
    long int *dynamicMap = NULL;

    fgets(buffer, 500, readPtr); //seeds
    int charRead = 0, secondIndex = 0, totalCharRead = 6;
    long int temp;
    while(sscanf(buffer+totalCharRead, " %ld%n", &temp, &charRead)>0)
    {
        totalCharRead += charRead;
        if(secondIndex>=size2)
        {
            printf("Not enough space in seed array.");
            break;
        }
        seedArr[secondIndex++] = temp;
    } 

    while(fgets(buffer, 500, readPtr) != NULL)
    {
        //start reading map after semicolon.
        if(strchr(buffer, ':'))
        {
            isMap = 1;
            index++;
            continue;
        }
        //stop current map reading after end of current map is reached.
        //don't read the seeds either.
        if(!strcmp(buffer, "\n") || !isMap)
        {
            isMap = 0;
            continue;
        }
        
        //anything below this is the map.
        map_Size[index]++;
        counter+=3;

        //for when dynamic memory not enough/not initialised.
        if(dynamicMap == NULL)
        {
            dynamicMap = malloc(24); //24 to represent size of 3 long int elements.
            currentArrSize = 24;
        }
        if(currentArrSize < counter*8) 
        {
            currentArrSize *=2;
            dynamicMap = realloc(dynamicMap, currentArrSize);
        }

        sscanf(buffer, " %ld %ld %ld", &dynamicMap[counter-3], &dynamicMap[counter-2], &dynamicMap[counter-1]);
    }
    return dynamicMap;
}

long int RunMap(long int number, int map_Index, int *map_Size, long int *dynamicArr)
{
    //find the base index for current map.
    //if refering to the 3rd map(index 2), then add up all the lines from map 1 and 2, then multiply by 3. 
    int baseIndex = 0;
    long int result = 0;
    for(int i = 0; i<map_Index; i++)
    {
        baseIndex += map_Size[i];
    }
    baseIndex *= 3; //multiply by 3, because 3 elements per line.

    for(int i = 0; i<map_Size[map_Index]; i++)
    {
        result = ConvertNumber(number, dynamicArr[baseIndex], dynamicArr[baseIndex+1], dynamicArr[baseIndex+2]);
        baseIndex += 3;
        if(result != number) return result; //once valid map is hit.
    }
    return number; //no valid map found
}

long int ConvertNumber(long int number, long int destination, long int source, long int range)
{
    //out of range
    if(number<source || number>source+range-1)
    {
        return number; //represents invalid map.
    }

    return destination + (number-source); //99, 50, 98, 2 : returns 51 which is correct.
}