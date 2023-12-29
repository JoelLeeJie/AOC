#include <stdio.h>
#include <stdlib.h>
#include "pattern.h"

int main(void)
{

    FILE* readFile = fopen("input.txt", "r");
    if(readFile == NULL)
    {
        printf("Unable to open input file");
        return 0;
    }

    char buffer[400];
    int nextElement[400]; int counter = 0;
    int previousElement[400];
    //stores this line of patterns into a temp array, then gets its next extrapolated pattern and stores it in "nextElement" result array.
    while(fgets(buffer, 400, readFile) != NULL)
    {
        char *readPtr = buffer; 
        int numRead = 0, length = 0;
        int patternArr[200];
        while(sscanf(readPtr, " %d%n", &patternArr[length], &numRead)>0)
        {
            length++; //only add to length if successful.
            readPtr += numRead;
        }
        previousElement[counter] = ExtrapolateBackwardsPattern(patternArr, length);
        nextElement[counter++] = ExtrapolatePattern(patternArr, length);
    }
    long int sum = 0, backSum = 0;
    for(int i = 0; i<counter; i++)
    {
        sum += nextElement[i];
        backSum += previousElement[i];
    }
    printf("Forwards: %ld, Backwards: %ld\n", sum, backSum); 

    fclose(readFile);
}

//returns the next element in that pattern. Uses recursion.
int ExtrapolatePattern(int *patternArr, int length)
{
    for(int i = 0; i<length; i++)
    {
        printf("%d ", patternArr[i]);
    }
    printf("\n");
    //base case: All elements in pattern are 0, so next element is 0.
    for(int i = 0; i<length; i++)
    {
        if(patternArr[i]!=0) break;
        if(i==length-1) return 0;
    }
    int *tempArr = malloc(sizeof(patternArr[0])*(length-1)); //stores the underlying pattern

    for(int i = 0; i<length-1; i++)
    {
        /*
        0   3   6   9  12  15
          3   3   3   3   3
        */
        tempArr[i] = patternArr[i+1] - patternArr[i];
    }

    /*
        0   3   6   9  12  15
          3   3   3   3   3
        //will return (15 + 3) which is 18, the next element in patternArr.
    */
    int nextElement = patternArr[length-1] + ExtrapolatePattern(tempArr, length-1);
    free(tempArr);
    return nextElement;
}

//returns the previous element in that pattern. Uses recursion.
//NOTE: This is the same code as ExtrapolatePattern, just that it uses patternArr[0] - func(); instead of patternArr[length-1]+func().
//can choose to combine into one function(have an option to choose back or forwards), but it's left this way for the explanatory comments.:)
int ExtrapolateBackwardsPattern(int *patternArr, int length)
{
    for(int i = 0; i<length; i++)
    {
        printf("%d ", patternArr[i]);
    }
    printf("\n");
    //base case: All elements in pattern are 0, so previous element is 0.
    for(int i = 0; i<length; i++)
    {
        if(patternArr[i]!=0) break;
        if(i==length-1) return 0;
    }
    int *tempArr = malloc(sizeof(patternArr[0])*(length-1)); //stores the underlying pattern

    //get the underlying pattern first.
    for(int i = 0; i<length-1; i++)
    {
        /*
        0   3   6   9  12  15
          3   3   3   3   3
        */
        tempArr[i] = patternArr[i+1] - patternArr[i];
    }

    /*
       -3   |0   3   6   9  12  15
          3   |3   3   3   3   3
        //will return (0 - 3) which is -3, the prev element in patternArr.
    */
    //get the previous element of the underlying pattern, and use that to get the previous element of this pattern.
    int previousElement = patternArr[0] - ExtrapolateBackwardsPattern(tempArr, length-1); 
    free(tempArr);
    return previousElement;
}