#include <stdio.h>
#include <string.h>
#include "parts2.h"

/*
Instead of searching for a number and checking for symbol, search for '*' and check for number.
Store the start position of each number first, so as to check for duplicate numbers.
*/

int main(void)
{
    //TODO: change example.txt to input.txt
    FILE* readFile = fopen("example.txt", "r");
    if(readFile == NULL) 
    {
        printf("Unable to read file");
        return 0;
    }

    int lineLength = GetLineLength(readFile);
    
    int temp, sum = 0;
    while((temp = getc(readFile))!=EOF)
    {
        //If not a number, check if the previously stored number is valid, then add to sum if it is.
        //Zero out both tempNumber and isValid, in preparation for the next number.
        if(temp == '*')
        {
            sum+=GetGearRatio(readFile, lineLength);
        }
    }

    printf("\n\n%d", sum);
    fclose(readFile);
    return 0;
}

int GetNumberPos(FILE *readPtr, long int *gears, int lineLength)
{
    int temp = 0;
    while(temp >='0' && temp <='9' && (ftell(readPtr)%lineLength)<(lineLength-1)) //ensure number doesn't wrap around to previous line
    {
        fseek(readPtr, -2, SEEK_CUR);
        temp = getc(readPtr);
    }
    long int numberPos = ftell(readPtr);
    for(int j = 0; j<2; j++)
    {
        if(gears[j] == numberPos) break; //only store unique positions.
        if(gears[j] == 0)
        {
            gears[j] = numberPos;
            break;
        }
        if(j==1) return 0; //numberPos not yet stored, but there's no space, meaning invalid gear ratio >2.
    }
    return 1;
}

//returns gear ratio if valid, else returns 0. Call when asterisk is hit.
int GetGearRatio(FILE *readPtr, int lineLength)
{ 
    long int offset = 0; //indicates current ptr position in the file.
    long int gears[2] = {0}; //store unique number positions in this. If overflow, that means >2 numbers adjacent, so it becomes invalid.
    
    offset = ftell(readPtr);
    int linePos = offset%(lineLength); //if it's at the end of the line, then it'll be 141, or if at the start of the line it'll be 0.

    fseek(readPtr, offset-lineLength-((linePos<2)? linePos:2), SEEK_SET); //go to previous line, and back 1 or 2 spaces depending on if there's enough space.
    
    for(int i = 0; i<((linePos<2)? 2:3); ++i)
    {
        int temp = getc(readPtr);
        if(temp <'0' || temp > '9') continue;
        
        if(!GetNumberPos(readPtr, gears, lineLength)) return 0;

        //set back to normal.
        fseek(readPtr, offset-lineLength-((linePos<2)? linePos:2)+i, SEEK_SET);
    }

    fseek(readPtr, offset-((linePos<2)? linePos:2), SEEK_SET);
    for(int i = 0; i<((linePos<2)? 2:3); ++i)
    {
        int temp = getc(readPtr);
        if(temp <'0' || temp > '9') continue;
        
        if(!GetNumberPos(readPtr, gears, lineLength)) return 0;

        //set back to normal.
        fseek(readPtr, offset-((linePos<2)? linePos:2)+i, SEEK_SET);
    }

    fseek(readPtr, offset+lineLength-((linePos<2)? linePos:2), SEEK_SET);
    for(int i = 0; i<((linePos<2)? 2:3); ++i)
    {
        int temp = getc(readPtr);
        if(temp <'0' || temp > '9') continue;
        
        if(!GetNumberPos(readPtr, gears, lineLength)) return 0;
    }
    
    for(int i = 0; i<2; i++)
    {
        gears[i] = ConvertNumber(readPtr, gears[i]);
    }


    fseek(readPtr, offset, SEEK_SET); //reset back to normal pos.
    return gears[0]*gears[1];
}


//returns line length, +1 for newline.
int GetLineLength(FILE* readPtr)
{
    char tempBuffer[500] = {0};
    //Read up to and including newline.
    fgets(tempBuffer, 500, readPtr);
    int temp = 0;
    //get number of characters just read using %n. Does not include newline.
    sscanf(tempBuffer, "%s%n", tempBuffer, &temp);
    temp+=1; //account for newline
    //return file ptr to where it was originally.
    fseek(readPtr, -temp, SEEK_CUR);
    return temp;
}


int ConvertNumber(FILE *readPtr, int startPos)
{
    fseek(readPtr, startPos, SEEK_SET);
    int result = 0, temp = getc(readPtr);
    while(temp>='0'&&temp<='9')
    {
        result *= 10;
        result += temp - '0';
        temp = getc(readPtr);
    }
    return result;
}