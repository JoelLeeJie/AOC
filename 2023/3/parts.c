#include <stdio.h>
#include <string.h>
#include "parts.h"


int main(void)
{
    FILE* readFile = fopen("input.txt", "r");
    if(readFile == NULL) 
    {
        printf("Unable to read file");
        return 0;
    }

    int lineLength = GetLineLength(readFile);
    
    int temp, sum = 0, tempNumber = 0, isValid = 0;
    while((temp = getc(readFile))!=EOF)
    {
        //If not a number, check if the previously stored number is valid, then add to sum if it is.
        //Zero out both tempNumber and isValid, in preparation for the next number.
        if(temp < '0' || temp > '9')
        {
            if(isValid){
                sum += tempNumber;
                printf("%d      ", tempNumber);
            }
            
            tempNumber = 0;
            isValid = 0;
            continue;
        }

        //set to 1 if it's a number and has adjacent symbol.
        if(CheckAdjacentSymbol(readFile, lineLength)) isValid = 1; 
        
        tempNumber *= 10;
        tempNumber += temp - '0';
    }

    printf("\n\n%d", sum);
    fclose(readFile);
    return 0;
}


//returns 1 if there is an adjacent pre-defined symbol, else returns 0.
//adjacent symbols are left, right, up, down, and includes diagonals.
int CheckAdjacentSymbol(FILE *readPtr, int lineLength)
{ 
    long int offset = 0; //indicates current ptr position in the file.
    char buffer[500] = {0};
    
    offset = ftell(readPtr);
    int linePos = offset%(lineLength); //if it's at the end of the line, then it'll be 141, or if at the start of the line it'll be 0.

    fseek(readPtr, -linePos, SEEK_CUR); //set back to start of line.
    
    if(!fseek(readPtr, -lineLength, SEEK_CUR)) //check if able to go to previous line
    {
        fgets(buffer, 500, readPtr); //gets previous line.
        //checks upper left, upper mid, upper right.
        if(CheckSides(buffer, lineLength, linePos))
        {
            fseek(readPtr, offset, SEEK_SET); //reset back to normal pos.
            return 1;
        }
    }

    fgets(buffer, 500, readPtr); //gets current line
    if(CheckSides(buffer, lineLength, linePos))
    {
        fseek(readPtr, offset, SEEK_SET); //reset back to normal pos.
        return 1;
    }

    if(fgets(buffer, 500, readPtr)!=NULL) //checks if there's a next line.
    {
        if(CheckSides(buffer, lineLength, linePos))
        {
            fseek(readPtr, offset, SEEK_SET); //reset back to normal pos.
            return 1;
        }
    }

    fseek(readPtr, offset, SEEK_SET); //reset back to normal pos.
    return 0;
}

//returns 1 if it's a pre-defined symbol, else returns 0.
int isSymbol(char input)
{
    char symbols[] = {'*', '#', '@', '$', '+', '-', '*', '/', '&', '=', '%', '!', '^', '(', ')', '_'};
    for(size_t i = 0; i<sizeof(symbols)/sizeof(symbols[0]); i++)
    {
        if(input == symbols[i]) return 1;
    }
    return 0;
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

int CheckSides(char *buffer, int lineLength, int linePos)
{
    for(int i = linePos-2; i<=linePos; i++)
    {
        i = (i<0)? 0:i; //check if negative
        i = (i>=lineLength)? lineLength:i; //check if out of bounds.
        if(isSymbol(buffer[i])) return 1;
    }
    return 0;
}