#include <stdio.h>

//indicates direction starting from upwards, in a clock-wise manner.
typedef enum Direction {NORTH, EAST, SOUTH, WEST, NONE} Direction;
typedef struct Coordinates {
    int x;
    int y;
}Coord;

Direction ReturnOppositeDirection(Direction direction);
char CheckCurrentPosition(FILE* readPtr);
int CheckLineLength(FILE* readPtr);
void MovePointer(FILE* readPtr, Direction direction, int lineLength);
int AutoMovePointer(FILE *readPtr, int lineLength, Direction *prevDirection, long int *currentPos);

Coord CheckCurrentCoordinates(FILE* readPtr, int lineLength);
int isGradientEqual(Coord left, Coord middle, Coord right);
