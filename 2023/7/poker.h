#include <stdio.h>
//defines poker hand types according to priority, where FIVEKIND with index 7 is the biggest.
typedef enum type{NONE, HIGHCARD, ONEPAIR, TWOPAIR, THREEKIND, FULLHOUSE, FOURKIND, FIVEKIND}type;

typedef struct Hand{
    char cards[6]; //account for null terminator.
    int bet;
    type handType;
}Hand;

Hand* ReadHands(FILE* readPtr, int *counter);
type CheckType(const char *card);
void Swap(Hand *left, Hand *right);
int CompareHand(Hand left, Hand right);
void SortArray(Hand *handArr, int lowIndex, int highIndex);

