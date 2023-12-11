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
    }   
    printf("%ld\n\n", lowest);

    ////TODO: PART 2
    //pass in the min, max of each seed pair. Do for all seed pairs.
    lowest = __LONG_MAX__;
    long int temp;
    for(int pair = 0; pair<num_Seeds; pair+=2)
    {
        if(seedArr[pair] <= 0 && seedArr[pair+1] <= 0) continue; 
        long int start = seedArr[pair];
        long int end = start+seedArr[pair+1]-1;
        temp = CheckMap(start, end, 0, map_Size, dynamicMaps);
        lowest = (temp<lowest)? temp:lowest;
    }
    printf("%ld\n", lowest);
    
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

    //run each map until valid number is returned.
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

//takes in a range, and gives lowest number for that range.
//recursive method(?).
long int CheckMap(long int min, long int max, int map_Index, int *map_Size, long int *dynamicMaps)
{
    /*
        if the seed range is 79 - 92, and the map is 52, 50, 48
        Then the destination range will be ConvertNumber(79) to ConvertNumber(92), or rather 81 - 94.
        Instead of converting each number individually, just convert the ends of the ranges.
        However, if the seed range does not fully fit into the map range, e.g. 45 - 60.
        Separate into 45 - 49, and 50 - 60. Store both ranges into an array.
        50 - 60 will become 52 - 62; For range 45-49, search for another map that can take or split it. 
        
        Since it's recursion, once the range has been fully converted, and there's no more maps remaining,
        then return the minimum of the converted range.
    */
    if(map_Size[map_Index] <= 0) return min; //recursion base condition is when no more maps remain.

    SeedPair seedPair[30] = {0};
    seedPair[0].min = min;
    seedPair[0].max = max;

    int baseIndex = 0; //baseIndex represents the starting index of the current map's first line.
    for(int i = 0; i<map_Index; i++)
    {
        baseIndex += map_Size[i];
    }
    baseIndex *=3;

    //iterating through all lines within the current map. 
    //if a pair is split, the split pair logically did not fit in any of the maps before, so no need iterate all over again.
    int currentPair = 0;
    for(int i = 0; i<map_Size[map_Index] && (seedPair[currentPair].min != 0 || seedPair[currentPair].max != 0); i++) 
    {
        long int mapDest = dynamicMaps[baseIndex + i*3];
        long int mapStart = dynamicMaps[baseIndex + i*3+1];
        long int mapEnd = mapStart + dynamicMaps[baseIndex + i*3+2] - 1; 

        //map does not match any part of the seed range, check next map
        if(seedPair[currentPair].min>mapEnd || seedPair[currentPair].max<mapStart)
        {
            continue;
        }
        //matches map in its entirety.
        if(seedPair[currentPair].min>=mapStart && seedPair[currentPair].max<=mapEnd)
        {
            seedPair[currentPair].max += mapDest-mapStart; //convert number.
            seedPair[currentPair].min += mapDest-mapStart;
            currentPair++;
            continue; //continue checking other seedPairs, if no more seedPairs the for loop will close automatically.
        }
        //seed range encompasses map range on both ends.
        if(seedPair[currentPair].min<mapStart && seedPair[currentPair].max>mapEnd)
        {
            //create 3 pairs. min to mapStart-1, mapStart to mapEnd, mapEnd+1 to max.
            AddSeedRange(seedPair, 30, seedPair[currentPair].min, mapStart-1);
            AddSeedRange(seedPair, 30, mapEnd+1, seedPair[currentPair].max);
            
            seedPair[currentPair].min = mapStart + mapDest-mapStart;
            seedPair[currentPair].max = mapEnd + mapDest-mapStart;
            currentPair++;
            continue; //continue checking unconverted pairs against other maps.
        }
        //anything below means either min or max is within, but not both.
        if(seedPair[currentPair].min>=mapStart)
        { //mapStart, min, mapEnd, max. 
            AddSeedRange(seedPair, 30, mapEnd+1, seedPair[currentPair].max);
            seedPair[currentPair].min += mapDest-mapStart; 
            seedPair[currentPair].max = mapEnd + mapDest-mapStart;
            currentPair++;
            continue;
        }
        else
        { //min, mapStart, max, mapEnd
            AddSeedRange(seedPair, 30, seedPair[currentPair].min, mapStart-1);
            seedPair[currentPair].min = mapStart + mapDest-mapStart; 
            seedPair[currentPair].max += mapDest-mapStart;
            currentPair++;
            continue;
        }
    }


    long int lowest = __LONG_MAX__;
    //iterate through all seedPairs. seedPairs should be separated and converted by this point.
    for(int i = 0; i<30; i++)
    {
        //no more seed pairs.
        if(seedPair[i].min == 0 && seedPair[i].max == 0)
        {
            break;
        }
        long int temp = CheckMap(seedPair[i].min, seedPair[i].max, map_Index+1, map_Size, dynamicMaps);
        lowest = (temp<lowest)? temp: lowest;
    }
    return lowest;
}

//check for empty space in the array and add a seedPair there.
void AddSeedRange(SeedPair *arr, int size, long int min, long int max)
{
    for(int i = 0; i<size; i++)
    {
        if(arr[i].min!=0 || arr[i].max != 0)
        {
            continue;
        }
        arr[i].min = min;
        arr[i].max = max;
        return;
    }
    printf("Not enough space in seedRangeArr\n");
}