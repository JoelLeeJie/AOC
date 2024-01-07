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

    //Check which directions loop back to S. Just keep moving the pointer until it reaches S, then divide by 2.
    //if it halts prematurely, it's not a valid loop. Do for all 4 directions.
    int num_Steps = 0;
    Direction validDir = NONE;
    for (int i = 0; i < 4; i++)
    {
        num_Steps = 0;
        MovePointer(readFile, (Direction)i, LINELENGTH);
        Direction prevDirection = ReturnOppositeDirection((Direction)i);
        long int currentPos = ftell(readFile);
        int temp = 0;
        while ((temp = AutoMovePointer(readFile, LINELENGTH, &prevDirection, &currentPos)) >= 1)
        {
            //printf("%c, temp: %d\n", CheckCurrentPosition(readFile), temp);
            num_Steps++;
            if (temp != 2)
                continue;
            //loop complete, print out direction.
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
            if(num_Steps>2) validDir = (Direction)i;
            break;
        }

        fseek(readFile, startPos, SEEK_SET); // reset back to start pos for next iteration.
    }

    //***PART 2***//
    //use shoelace theorem to check area, then reverse engineer using pick's theorem to find number of points within that area.
    fseek(readFile, startPos, SEEK_SET);
    Coord points[3] = {0}; //stores left, middle, right.
    Coord vertices[10000] = {0}; //stores all the vertices.

    points[1] = CheckCurrentCoordinates(readFile, LINELENGTH); //start position
    MovePointer(readFile, validDir, LINELENGTH);
    points[2] = CheckCurrentCoordinates(readFile, LINELENGTH); //the next tile from start position.
    int verticeCounter = 0;
    
    Direction prevDirection = ReturnOppositeDirection(validDir);
    long int currentPosition = ftell(readFile);
    int temp = 0;
    while((temp=AutoMovePointer(readFile, LINELENGTH, &prevDirection, &currentPosition)) >= 1)
    {
        points[0] = points[1]; 
        points[1] = points[2];
        points[2] = CheckCurrentCoordinates(readFile, LINELENGTH);
        if(!isGradientEqual(points[0], points[1], points[2]))
        {
            //That means the middle point is the vertice.
            vertices[verticeCounter++] = points[1];
        }
        if(temp==2) //currentPosition is S.
        {
            MovePointer(readFile, validDir, LINELENGTH);
            points[0] = points[1];
            points[1] = points[2];
            points[2] = CheckCurrentCoordinates(readFile, LINELENGTH);
            if(!isGradientEqual(points[0], points[1], points[2]))
            {
                vertices[verticeCounter++] = points[1];
            }
            break;
        }
    }
    
    double sum = 0;
    for(int i = 0; i<verticeCounter-1; i++)
    {
        printf("(%d, %d) ", vertices[i].x, vertices[i].y);
        sum += vertices[i].x * vertices[i+1].y - vertices[i].y * vertices[i+1].x;
        printf("Sum: %lf\n", sum);
        if(i == verticeCounter-2)
        {
            sum += vertices[i+1].x*vertices[0].y - vertices[i+1].y * vertices[0].x;
            sum*=0.5;
            printf("Sum: %lf\n", sum);
            break;
        }    
    }
    if(sum<0) sum*=-1;



    // Pick's thm: Area = num_interior + 0.5num_perimeter -1
    // num_interior = Area - 0.5num_perimeter+1
    printf("Answer: %f\n", (float)(sum - 0.5*(num_Steps+1)+1));

    
    fclose(readFile);
}

Coord CheckCurrentCoordinates(FILE* readPtr, int lineLength)
{
    int currentPosition = ftell(readPtr);
    return (Coord){currentPosition%lineLength, (currentPosition/lineLength)+1};
}

//returns 1 if gradients is equal, 0 if not.
int isGradientEqual(Coord left, Coord middle, Coord right)
{
    Coord gradientFirst = {middle.x-left.x, middle.y-left.y};
    Coord gradientSecond = {right.x-middle.x, right.y-middle.y};
    if(gradientFirst.x == gradientSecond.x && gradientFirst.y == gradientSecond.y) return 1;
    return 0;
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
        break;
    case 'S':
        return 2;
    }
    if (nextDir == NONE)
    {
        printf("Dead End\n");
        return 0;
    }
    MovePointer(readPtr, nextDir, lineLength);
    *prevDirection = ReturnOppositeDirection(nextDir);
    *currentPos = ftell(readPtr);
    if(CheckCurrentPosition(readPtr) == 'S') return 2; //check if it reached S after moving one tile.

    return 1;
}

