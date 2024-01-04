#include <stdio.h>

//indicates direction starting from upwards, in a clock-wise manner.
typedef enum Direction {NORTH, EAST, SOUTH, WEST, NONE} Direction;

//int CheckDirections(Direction prevDirection, FILE* readPtr, int lineLength);
Direction ReturnOppositeDirection(Direction direction);
char CheckCurrentPosition(FILE* readPtr);
int CheckLineLength(FILE* readPtr);
void MovePointer(FILE* readPtr, Direction direction, int lineLength);
int AutoMovePointer(FILE *readPtr, int lineLength, Direction *prevDirection, long int *currentPos);