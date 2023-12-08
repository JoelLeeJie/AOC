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
    FILE* readFile = fopen("input.txt", "r");
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
            printf("%d\n", GetGearRatio(readFile, lineLength));
        }
    }

    printf("\n%d\n", sum);
    fclose(readFile);
    return 0;
}

int GetNumber(FILE *readPtr, long int *gears, size_t sizeofArr) //called when readPtr is in front of a number.
{
    int offset = ftell(readPtr);
    int temp;
    
    //shift readPtr to behind the first digit of the number.
    do
    {
        if(fseek(readPtr, -2, SEEK_CUR))
        {
            fseek(readPtr, -1, SEEK_CUR);
            break;
        }
        temp = getc(readPtr);
    }while(temp>='0' && temp<='9');

    

    int result = 0; 
    temp = getc(readPtr);
    while(temp>='0'&&temp<='9')
    {
        result *= 10;
        result += temp - '0';
        temp = getc(readPtr);
    }

    fseek(readPtr, offset, SEEK_SET); //reset back to normal pos.
    
    for(size_t i = 0; i<sizeofArr; i++)
    {
        if(gears[i] == result) break; //don't store duplicates.
        if(gears[i] == -1) //empty 
        {
            gears[i] = result;
            break;
        }
        if(i == sizeofArr-1) return 0; //no empty space, not a duplicate, thus >2 gears making it invalid.
    }

    return 1;
}

//returns gear ratio if valid, else returns 0. Call when asterisk is hit.
int GetGearRatio(FILE *readPtr, int lineLength)
{ 
    long int offset = 0; //indicates current ptr position in the file.
    long int gears[2] = {-1, -1}; //store unique number positions in this. If overflow, that means >2 numbers adjacent, so it becomes invalid.
    
    offset = ftell(readPtr);
    int linePos = offset%(lineLength); //if it's at the end of the line, then it'll be 141, or if at the start of the line it'll be 0.

    fseek(readPtr, offset-lineLength-((linePos<2)? linePos:2), SEEK_SET); //go to previous line, and back 1 or 2 spaces depending on if there's enough space.
    for(int i = 0; i<((linePos<2)? 2:3); ++i)
    {
        int temp = getc(readPtr);
        if(temp <'0' || temp > '9') continue;
        
        if(!GetNumber(readPtr, gears, lineLength)) 
        {
            printf("More than 2 gears, invalid\n");
            return 0;
        }

    }

    fseek(readPtr, offset-((linePos<2)? linePos:2), SEEK_SET);
    for(int i = 0; i<((linePos<2)? 2:3); ++i)
    {
        int temp = getc(readPtr);
        if(temp <'0' || temp > '9') continue;
        
        if(!GetNumber(readPtr, gears, lineLength)) return 0;

    }

    fseek(readPtr, offset+lineLength-((linePos<2)? linePos:2), SEEK_SET);
    for(int i = 0; i<((linePos<2)? 2:3); ++i)
    {
        int temp = getc(readPtr);
        if(temp <'0' || temp > '9') continue;
        
        if(!GetNumber(readPtr, gears, lineLength)) return 0;
    }
    


    fseek(readPtr, offset, SEEK_SET); //reset back to normal pos.
    
    for(size_t i = 0; i<sizeof(gears)/sizeof(gears[0]); i++)
    {
        if(gears[i] == -1) return 0; //<2 gears, invalid.
    }
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
