#include <stdio.h>
#include "trebuchet.h"



int main(void)
{
    run("input.txt");
    return 0;
}

void run(const char* inputFile)
{
    FILE* readFile = fopen(inputFile, "r");
    if(readFile == NULL)
    {
        printf("Read File is NULL.");
        return;
    }
    char buffer[300] = {0};
    long int sum = 0;
    while(fgets(buffer, 300, readFile))
    {
        sum += parseString(buffer); 
    }
    fclose(readFile);
    printf("SUM: %ld", sum);
}

int parseString(const char* inputLine)
{
    int first = 0, last = 0, isFirstSet = 0;
    char temp;
    while(*inputLine) //run until null terminator of string.
    {

        temp = *inputLine;
        
        //check if it's a number character.
        if(temp < '0' || temp > '9') 
        {
            //check if it's a number literal
            if(CheckNumber(inputLine) == -1)
            {
                inputLine++;
                continue;
            }
            else
            {
                temp = '0' + CheckNumber(inputLine);  //convert to number character.
            }
        }     
        
        //anything below this means temp is a valid number.
        if(!isFirstSet) //if first number has not been set. Cannot check for first = 0, as 0 is a valid input.
        {
            first = temp - '0';
            last = first; //set last to first, jic last is not set later on.
            isFirstSet = 1;
        }
        else //set the last number to the latest number.
        {
            last = temp - '0';
        }
        inputLine++;
    }
    return first*10+last;
}

/*
From that character position in input, check against pre-defined arr, to see if it matches any of the strings in there. 
*/
int CheckNumber(const char* input)
{
    //define an array of char* (an array of strings).
    char *arr[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    for(size_t i = 0; i<sizeof(arr)/8; i++)
    {
        //return index+1, if it exists in the string.
        if(StringCompare(arr[i], input)) return i+1;
    }
    return -1;
}

int StringCompare(const char* nullTerminated_leftStr, const char* rightStr)
{
    //run loop until null-terminator
    while(*nullTerminated_leftStr)
    {
        if(*nullTerminated_leftStr++ != *rightStr++) //will also activate if rightStr hits null terminator early.
        {
            return 0;
        }
    }
    return 1; 
}
