#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int *GetArray(const char *srcLine, int *size);
int CountPoints(const int *winArr, int sizeWin, const int *ownArr, int sizeOwn);



int main(void)
{
    FILE *readFile = fopen("input.txt", "r");
    if(readFile == NULL)
    {
        printf("Unable to open input file.");
        return 0;
    }

    char buffer[500] = {0};
    int sum = 0;
    char secondBuffer[500] = {0};
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

        sum += CountPoints(winDynamicArray, sizeWinArr, ownDynamicArray, sizeOwnArr);
        free(winDynamicArray);
        free(ownDynamicArray);
    }
    printf("%d", sum);
    return 0;
}

int *GetArray(const char *srcLine, int *size)
{
    int temp, charRead = 0;
    int current_allocSize = 0;
    int *allocArray;
    while(sscanf(srcLine, " %d%n", &temp, &charRead)) //increment readPtr by number of characters read.
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


int CountPoints(const int *winArr, int sizeWin, const int *ownArr, int sizeOwn)
{
    int points = 0;
    for(int own = 0; own<sizeOwn; own++)
    {
        for(int win = 0; win<sizeWin; win++)
        {
            if(ownArr[own] == winArr[win])
            {
                printf("%d : %d", ownArr[own], winArr[win]);
                points++;
                break; //break inner loop, carry on to next.
            }
        }
    }
    printf("\n");
    return points;
}