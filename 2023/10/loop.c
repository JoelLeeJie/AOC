#include "loop.h"

int main(void)
{
    FILE *readFile = fopen("input.txt", "r");
    if (readFile == NULL)
    {
        printf("Unable to read input file");
        return 0;
    }
    const int LINELENGTH = CheckLineLength(readFile);

    // sets read pointer to be behind S.
    while (getc(readFile) != 'S');
    fseek(readFile, -1, SEEK_CUR);
    long int startPos = ftell(readFile);

    //Check which directions loop back to S.
    for (int i = 0; i < 4; i++)
    {
        MovePointer(readFile, (Direction)i, LINELENGTH);
        Direction prevDirection = ReturnOppositeDirection((Direction)i);
        long int currentPos = ftell(readFile);
        int temp = 0, num_Steps = -1;
        while ((temp = AutoMovePointer(readFile, LINELENGTH, &prevDirection, &currentPos)) >= 1)
        {
            num_Steps++;
            if (temp != 2)
                continue;
            //loop complete, print out direction.
            num_Steps = (num_Steps-1)/2 +1; //Take total steps and divide by 2 to find center.
            switch (i)
            {
            case 0:
                printf("North, number of steps: %d\n", num_Steps);
                break;
            case 1:
                printf("East, number of steps: %d\n", num_Steps);
                break;
            case 2:
                printf("South, number of steps: %d\n", num_Steps);
                break;
            case 3:
                printf("West, number of steps: %d\n", num_Steps);
                break;
            }
            break;
        }

        fseek(readFile, startPos, SEEK_SET); // reset back to start pos for next iteration.
    }


    fclose(readFile);
}



Direction ReturnOppositeDirection(Direction direction)
{
    switch (direction)
    {
    case NORTH:
        return SOUTH;
    case SOUTH:
        return NORTH;
    case EAST:
        return WEST;
    case WEST:
        return EAST;
    case NONE:
        return NONE;
    default:
        break;
    }
    return NONE;
}

char CheckCurrentPosition(FILE *readPtr)
{
    char temp = getc(readPtr);
    fseek(readPtr, -1, SEEK_CUR);
    return temp;
}

// returns line length up to and including newline character. Sets file pointer back to start.
int CheckLineLength(FILE *readPtr)
{
    int counter = 1;
    fseek(readPtr, 0, SEEK_SET);
    while (getc(readPtr) != '\n')
    {
        counter++;
    }
    fseek(readPtr, 0, SEEK_SET);
    return counter;
}

void MovePointer(FILE *readPtr, Direction direction, int lineLength)
{
    switch (direction)
    {
    case NORTH:
        fseek(readPtr, -lineLength, SEEK_CUR);
        break;
    case EAST:
        fseek(readPtr, 1, SEEK_CUR);
        break;
    case SOUTH:
        fseek(readPtr, lineLength, SEEK_CUR);
        break;
    case WEST:
        fseek(readPtr, -1, SEEK_CUR);
        break;
    default:
        break;
    }
}

// moves the current read pointer based on the pipe it is at.
// if successful, return 1. If it loops back to start, return 2.
int AutoMovePointer(FILE *readPtr, int lineLength, Direction *prevDirection, long int *currentPos)
{
    fseek(readPtr, *currentPos, SEEK_SET); // so that it can be run simultaneously.

    char currentChar = CheckCurrentPosition(readPtr);
    // loop back to start.
    if (currentChar == 'S')
    {
        return 2;
    }

    Direction nextDir = NONE;
    switch (currentChar)
    {
    case '|': // North-South
        if (*prevDirection == NORTH)
        {
            nextDir = SOUTH;
        }
        else if (*prevDirection == SOUTH)
        {
            nextDir = NORTH;
        }
        break;
    case '-': // East-West
        if (*prevDirection == EAST)
        {
            nextDir = WEST;
        }
        else if (*prevDirection == WEST)
        {
            nextDir = EAST;
        }
        break;
    case 'L': // North-East
        if (*prevDirection == NORTH)
        {
            nextDir = EAST;
        }
        else if (*prevDirection == EAST)
        {
            nextDir = NORTH;
        }
        break;
    case 'J': // North-West
        if (*prevDirection == NORTH)
        {
            nextDir = WEST;
        }
        else if (*prevDirection == WEST)
        {
            nextDir = NORTH;
        }
        break;
    case 'F': // South-East
        if (*prevDirection == SOUTH)
        {
            nextDir = EAST;
        }
        else if (*prevDirection == EAST)
        {
            nextDir = SOUTH;
        }
        break;
    case '7': // South-West
        if (*prevDirection == SOUTH)
        {
            nextDir = WEST;
        }
        else if (*prevDirection == WEST)
        {
            nextDir = SOUTH;
        }
        break;
    case '.': // Unable to move
        nextDir = NONE;
    }
    if (nextDir == NONE)
    {
        printf("Dead End\n");
        return 0;
    }
    MovePointer(readPtr, nextDir, lineLength);
    *prevDirection = ReturnOppositeDirection(nextDir);
    *currentPos = ftell(readPtr);
    return 1;
}

/*
int CheckDirections(Direction prevDirection, FILE *readPtr, int lineLength)
{
    // Check current position, and check only in that direction to see if can continue.
    // e.g. If it's a 7, and prevDirection is south, then check west but not south.
    // base case would be returning back to S, in which case return 1, or hitting dead end(return 0).

    // base case when loops back to 'S'. If prevDirection == NONE, that is the start case.

    long int currentPosition = ftell(readPtr);
    char currentChar = CheckCurrentPosition(readPtr);

    if (currentChar == 'S' && prevDirection != NONE)
    {
        return 1;
    }

    if (currentChar == 'S')
    {
        // check in all 4 directions
        for (int i = 0; i < 4; i++)
        {
            switch (i)
            {
            case 0:
                printf("North: ");
                break;
            case 1:
                printf("East: ");
                break;
            case 2:
                printf("South: ");
                break;
            case 3:
                printf("West: ");
                break;
            }
            MovePointer(readPtr, (Direction)i, lineLength);
            printf("%d\n", CheckDirections(ReturnOppositeDirection((Direction)i), readPtr, lineLength));
            fseek(readPtr, currentPosition, SEEK_SET);
        }
        return 0;
    }

    // TODO: remb to check for starting 'S'.
    int returnVal = 0;
    Direction nextDir = NONE;
    // Checks if current Pipe and previous direction is valid(e.g. '-' and prevDirection == North is not valid.)
    switch (currentChar)
    {
    case '|': // North-South
        if (prevDirection == NORTH)
        {
            nextDir = SOUTH;
        }
        else if (prevDirection == SOUTH)
        {
            nextDir = NORTH;
        }
        break;
    case '-': // East-West
        if (prevDirection == EAST)
        {
            nextDir = WEST;
        }
        else if (prevDirection == WEST)
        {
            nextDir = EAST;
        }
        break;
    case 'L': // North-East
        if (prevDirection == NORTH)
        {
            nextDir = EAST;
        }
        else if (prevDirection == EAST)
        {
            nextDir = NORTH;
        }
        break;
    case 'J': // North-West
        if (prevDirection == NORTH)
        {
            nextDir = WEST;
        }
        else if (prevDirection == WEST)
        {
            nextDir = NORTH;
        }
        break;
    case 'F': // South-East
        if (prevDirection == SOUTH)
        {
            nextDir = EAST;
        }
        else if (prevDirection == EAST)
        {
            nextDir = SOUTH;
        }
        break;
    case '7': // South-West
        if (prevDirection == SOUTH)
        {
            nextDir = WEST;
        }
        else if (prevDirection == WEST)
        {
            nextDir = SOUTH;
        }
        break;
    case '.': // Unable to move
        nextDir = NONE;
    }
    if (nextDir == NONE)
        return 0; // invalid pipe or prevDirection, unable to move further.

    MovePointer(readPtr, nextDir, lineLength);
    returnVal = CheckDirections(ReturnOppositeDirection(nextDir), readPtr, lineLength);

    // set back to normal after calling below recursions.
    fseek(readPtr, currentPosition, SEEK_SET);
    return returnVal;
}

*/