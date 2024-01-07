#include "galaxy.h"

int main(void)
{
    FILE* readFile = fopen("input.txt", "r");
    if(readFile == NULL)
    {
        printf("Unable to open input file\n");
        return 0;
    }
    const int LINELENGTH = GetLineLength(readFile);

    int emptyColumn[500] = {0}, num_emptyColumn = 0;
    int emptyRow[500] = {0}, num_emptyRow = 0;
    for(int i = 0; i<=LINELENGTH; i++)
    {
        if(CheckEmptyColumn(readFile, LINELENGTH))
        {
            emptyColumn[num_emptyColumn++] = i; 
        }
        getc(readFile);
    }

    fseek(readFile, 0, SEEK_END);
    long int endFile = ftell(readFile);
    fseek(readFile, 0, SEEK_SET);

    for(int i = 0; ;i++)
    {
        if(CheckEmptyRow(readFile))
        {
            emptyRow[num_emptyRow++] = i;
        }
        fseek(readFile, LINELENGTH, SEEK_CUR);
        if(ftell(readFile)>endFile)
        {
            fseek(readFile, 0, SEEK_SET);
            break;
        }
    }

    /*
    Empty rows and columns found. Add them to answer if it's between the pair of coordinates.
    */
    Coord galaxyPosition[3000] = {0}; int num_Galaxies = 0;
    char temp;
    while((temp = getc(readFile))!=EOF)
    {
        if(temp == '#')
        {
            fseek(readFile, -1, SEEK_CUR);
            galaxyPosition[num_Galaxies++] = GetCurrentCoordinates(readFile, LINELENGTH);
            fseek(readFile, 1, SEEK_CUR);
        }
    }

    int num_IntersectingColumn = 0, num_IntersectingRow = 0; 
    long int sum = 0;
    for(int first = 0; first<num_Galaxies; first++)
    {
        for(int second = first; second<num_Galaxies; second++)
        {
            if(first == second) continue;
            CheckNumberIntersectingEmptyRowAndColumn(
                galaxyPosition[first], galaxyPosition[second],
                emptyColumn, num_emptyColumn, 
                emptyRow, num_emptyRow,
                &num_IntersectingColumn, &num_IntersectingRow
            );
            long int temp = sum;
            sum += Abs(galaxyPosition[first].x-galaxyPosition[second].x) + 999999 * num_IntersectingColumn;
            sum += Abs(galaxyPosition[first].y-galaxyPosition[second].y) + 999999 * num_IntersectingRow;  
            temp = sum - temp;
        }
    }

    printf("%ld\n", sum);
    fclose(readFile);
}

int Abs(int num)
{
    return (num<0)? num*-1: num;
}


//returns the number of empty rows and columns that are in between the pair of coordinates.
void CheckNumberIntersectingEmptyRowAndColumn(
    Coord A, Coord B, 
    int *emptyColumn, int num_emptyColumn, 
    int *emptyRow, int num_emptyRow,
    int *num_IntersectingColumn, int *num_IntersectingRow
    )
{
    *num_IntersectingColumn = *num_IntersectingRow = 0;
    int minX, maxX, minY, maxY;
    if(A.x<B.x)
    {
        minX = A.x;
        maxX = B.x;
    }
    else
    {
        minX = B.x;
        maxX = A.x;
    }
    if(A.y<B.y)
    {
        minY = A.y;
        maxY = B.y;
    }
    else
    {
        minY = B.y;
        maxY = A.y;
    }
    
    //Check if empty column is in between the x coordinates.
    for(int i = 0; i<num_emptyColumn; i++)
    {
        if(emptyColumn[i] >= minX && emptyColumn[i] <= maxX)
        {
            ++*num_IntersectingColumn;
        }
    }
    //Check if empty row is in between the y coordinates.
    for(int i = 0; i<num_emptyRow; i++)
    {
        if(emptyRow[i] >= minY && emptyRow[i] <= maxY)
        {
            ++*num_IntersectingRow;
        }
    }
}


Coord GetCurrentCoordinates(FILE* readPtr, int lineLength)
{
    int currentPos = ftell(readPtr);
    return (Coord){
        currentPos%lineLength,
        currentPos/lineLength
    };
}

//returns number of characters in a line up to and including newline character.
int GetLineLength(FILE* readPtr)
{
    fseek(readPtr, 0, SEEK_SET);
    int length = 0;
    while(getc(readPtr)!='\n') //read until newline character
    {
        length++;
    }
    fseek(readPtr, 0, SEEK_SET);
    return length+1;
}

char CheckCurrentPosition(FILE* readPtr)
{
    char temp = getc(readPtr);
    fseek(readPtr, -1, SEEK_CUR);
    return temp;
}

//returns 1 if current column only contains '.', else returns 0.
int CheckEmptyColumn(FILE* readPtr, int lineLength)
{
    long int startPos = ftell(readPtr);
    fseek(readPtr, 0, SEEK_END);
    long int endFile = ftell(readPtr);
    fseek(readPtr, startPos, SEEK_SET);
    do{
        if(CheckCurrentPosition(readPtr)!='.')
        {
            fseek(readPtr, startPos, SEEK_SET);
            return 0;
        }
        fseek(readPtr, lineLength, SEEK_CUR);
        if(ftell(readPtr)>endFile)
        {
            break;
        }
        
    }
    while(1); //Check every element below it until it reaches the end of the file.
    fseek(readPtr, startPos, SEEK_SET);
    return 1;
}

//returns 1 if current row only contains '.', else returns 0.
int CheckEmptyRow(FILE* readPtr)
{
    long int startPos = ftell(readPtr);
    char buffer[500] = {0};
    fgets(buffer, 500, readPtr);
    fseek(readPtr, startPos, SEEK_SET);

    for(int i = 0; i<500 && buffer[i]!='\0' && buffer[i] !='\n'; i++)
    {
        if(buffer[i] != '.') return 0;
    }
    return 1;
}


