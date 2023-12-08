#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "math.h"

int *GetArray(const char *srcLine, int *size);
int CountPoints(const int *winArr, int sizeWin, const int *ownArr, int sizeOwn, int *pointCount);
long int CountTotalCard(int *cardArray, int numberCards);



int main(void)
{
    FILE *readFile = fopen("input.txt", "r");
    if(readFile == NULL)
    {
        printf("Unable to open input file.");
        return 0;
    }

    char buffer[500] = {0};
    int sum = 0, temp = 0;
    char secondBuffer[500] = {0};

    int cardCounter = 0, currentAlloc = 8;
    int *cardSum_DynamicArray = malloc(8);
    while(fgets(buffer, 500, readFile)!= NULL)
    {
        int *winDynamicArray, *ownDynamicArray;
        int sizeWinArr = 0, sizeOwnArr = 0;
        char *token;

        token = strtok(buffer, "|");
        memset(secondBuffer, 0, 500);
        sscanf(token, "%*[^:]: %[^\n]", secondBuffer); //remove the card indexing
        winDynamicArray = GetArray(secondBuffer, &sizeWinArr);

        token = strtok(NULL, "|");
        ownDynamicArray = GetArray(token, &sizeOwnArr);


        sum += CountPoints(winDynamicArray, sizeWinArr, ownDynamicArray, sizeOwnArr, &temp);
        free(winDynamicArray);
        free(ownDynamicArray);

        cardCounter++;
        if(currentAlloc<cardCounter*8) //not enough space to store all the cards currently.
        {
            cardSum_DynamicArray = realloc(cardSum_DynamicArray, currentAlloc * 2);
            currentAlloc *= 2;
        }
        cardSum_DynamicArray[cardCounter-1] = temp;
    }
    printf("%d\n", sum);
    printf("%ld\n", CountTotalCard(cardSum_DynamicArray, cardCounter));
    free(cardSum_DynamicArray);
    return 0;
}

long int CountTotalCard(int *cardArray, int numberCards)
{
    long int sum = 0, matchingNumbers = 0;
    for(int i = 0; i<numberCards; i++)
    {
        printf("%d, ", cardArray[i]);
    }
    printf("\n");

    //change array from back to front.
    for(int i = numberCards-1; i>=0; --i)
    {
        //If card[5] has 2 matching, it'll add card[6] and card[7] to itself.
        matchingNumbers = cardArray[i];
        for(int j = 1; j<=matchingNumbers && j+i<numberCards; j++) //don't go out of bounds of array.
        {
            cardArray[i] += cardArray[i+j];
        }
    }

    for(int i = 0; i<numberCards; i++)
    {
        sum+= cardArray[i]+1; //+1 to account for the original card too.
        printf("%d, ", cardArray[i]);
    }
    printf("\n");
    return sum;
}

int *GetArray(const char *srcLine, int *size)
{
    int temp, charRead = 0;
    int current_allocSize = 0;
    int *allocArray;
    while(sscanf(srcLine, " %d%n", &temp, &charRead) > 0) //increment readPtr by number of characters read.
    {
        srcLine += charRead;
        ++*size; //number of integers currently stored, including the one just read.
        if(current_allocSize == 0)
        {
            allocArray = malloc(8);
            current_allocSize = 8;
        }
        if(current_allocSize<*size * 8) //insufficient space in array.
        {
            allocArray = realloc(allocArray, current_allocSize*2); //double space in array.
            current_allocSize *= 2;
        }
        allocArray[*size-1] = temp;
    }
    return allocArray;
}


int CountPoints(const int *winArr, int sizeWin, const int *ownArr, int sizeOwn, int *pointCount)
{
    int points = 0;
    for(int own = 0; own<sizeOwn; own++)
    {
        for(int win = 0; win<sizeWin; win++)
        {
            if(ownArr[own] == winArr[win])
            {
                points++;
                break; //break inner loop, carry on to next.
            }
        }
    }
    *pointCount = points;
    return pow(2, points-1);
}