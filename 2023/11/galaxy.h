#include <stdio.h>
typedef struct Coordinates{
    int x;
    int y;
}Coord;

int GetLineLength(FILE* readPtr);
char CheckCurrentPosition(FILE* readPtr);
int CheckEmptyColumn(FILE* readPtr, int lineLength);
int CheckEmptyRow(FILE* readPtr);
int Abs(int num);
void CheckNumberIntersectingEmptyRowAndColumn(
    Coord A, Coord B, 
    int *emptyColumn, int num_emptyColumn, 
    int *emptyRow, int num_emptyRow,
    int *num_IntersectingColumn, int *num_IntersectingRow
    );
Coord GetCurrentCoordinates(FILE* readPtr, int lineLength);
