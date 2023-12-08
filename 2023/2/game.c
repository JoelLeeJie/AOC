#include <stdio.h>
#include <string.h>

void ParseString(char *buffer, int inputArr[]);

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Need at least 3 arguments. Red Green Blue");
        return 0;
    }
    int colourMax[3], inputColour_Max[3] = {0}; // rgb
    for (int i = 0; i < 3; i++)
    {
        sscanf(argv[i + 1], "%d", &colourMax[i]);
    }

    FILE *readFile = fopen("input.txt", "r");
    if (readFile == NULL)
    {
        printf("Unable to open file");
        return 0;
    }

    // Reads each line, store max of each input colour, then check if it exceeds colourMax.
    // If it's a valid game, add it to sum.
    char buffer[400];
    int sum = 0, counter = 0;
    long int cubeminsum = 0;
    while (fgets(buffer, 400, readFile) != NULL)
    {
        counter++;
        ParseString(buffer, inputColour_Max);
        // do not add counter to sum if it's not a valid game.
        for (int i = 0; i < 3; i++)
        {
            if (inputColour_Max[i] > colourMax[i])
            {
                sum -= counter;
                break;
            }
            if(inputColour_Max[i] == 0) printf("hi");
        }
        sum += counter;
        cubeminsum = cubeminsum + inputColour_Max[0] * inputColour_Max[1] * inputColour_Max[2];
        printf("%d %d %d\n\n", inputColour_Max[0], inputColour_Max[1], inputColour_Max[2]);
        if((inputColour_Max[0] * inputColour_Max[1] * inputColour_Max[2])<=1) printf("sus\n");
    }

    fclose(readFile);
    printf("%d\n", sum);
    printf("%ld\n", cubeminsum);
    return 0;
}

// Stores the max of each colour into input arr.
void ParseString(char *buffer, int inputArr[])
{
    memset(inputArr, 0, 3*sizeof(inputArr[0]));
    char *tokenPtr;
    int temp = 0;
    char tempStr[10], *colours[] = {"red", "green", "blue"};

    while(*buffer++ != ':'); //move buffer past the semicolon, postfix ++ intended.

    // divide the entire string into individual sections, using ";" and ","
    tokenPtr = strtok(buffer, ",;");
    while (tokenPtr != NULL)
    {
        sscanf(tokenPtr, "%d %s", &temp, tempStr);
        printf("%d ", temp);
        for (size_t i = 0; i < sizeof(colours) / 8; i++)
        {
            // if it matches that colour, and is more than maximum, store as the new maximum for that colour
            if (!strcmp(colours[i], tempStr) && temp > inputArr[i])
                inputArr[i] = temp;
        }
        tokenPtr = strtok(NULL, ",;");
    }
    printf("\n");
}
