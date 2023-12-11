#include <stdio.h>

/*
    Boat
    1. Starting speed of 0
    2. Acceleration of 1/s held down.

    Assuming total race time is given as t, let time held down be x, where x<t.  
    distance traveled = velocity * time = (x * acceleration)(t-x);
*/

#define maxRaces 40
int startSpeed = 0, acceleration_perSecondHeld = 1;


int main(void)
{
    long int record[maxRaces][2] = {0}; //time, distance
    FILE *readFile = fopen("input.txt", "r");
    if(readFile == NULL)
    {
        printf("Unable to read input file");
        return 0;
    }

    char buffer[500] = {0}, *readPtr = buffer;
    int charRead = 0, numberOfRaces = 0;

    //get time, then get distance.
    for (int j = 0; j < 2; j++)
    {
        fgets(buffer, 500, readFile);
        for (readPtr = buffer; *readPtr != ':' && *readPtr != '\0'; readPtr++); // move readPtr past the semicolon.
        readPtr++;
        for (int i = 0; sscanf(readPtr, " %ld%n", &record[i][j], &charRead) > 0; i++)
        {
            readPtr += charRead;
            if(i>=maxRaces)
            {
                printf("Not enough space in array\n");
                return 0;
            }
            numberOfRaces = i+1;
        }
    }


    //iterate through all race records.
    int numOfWays[maxRaces] = {0}, raceIndex = 0;
    for(raceIndex = 0; record[raceIndex][0]>0 && raceIndex<maxRaces; raceIndex++)
    {
        long int recordTime = record[raceIndex][0];
        long int recordDistance = record[raceIndex][1];
        for(long int timeHeld = 0; timeHeld<=recordTime; timeHeld++)
        {
            if((timeHeld * acceleration_perSecondHeld * (recordTime-timeHeld))<=recordDistance)
            {
                continue;
            }
            numOfWays[raceIndex]++;             
        }
    }

    int sum = numOfWays[0];
    for(int i = 1; i<raceIndex; i++)
    {
        sum*= numOfWays[i];
    }
    printf("%d\n", sum);

    for(int j = 0; j<2; j++)
    {
        for(int i = 0; i<numberOfRaces; i++)
        {
            int temp = record[i][j];
            do
            {
                record[numberOfRaces][j] *= 10;
            } while(temp/=10);
            record[numberOfRaces][j] += record[i][j];
        }
    }

    sum = 0;
    for(long int timeHeld = 0, recordTime = record[numberOfRaces][0], recordDistance = record[numberOfRaces][1]; timeHeld<=recordTime; timeHeld++)
    {
        if((timeHeld * acceleration_perSecondHeld * (recordTime-timeHeld))<=recordDistance)
        {
            continue;
        }
        sum++;
    }
    printf("%d\n", sum);

    fclose(readFile);
    return 0;
}