#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "poker.h"

int main(void)
{
    FILE* readFile = fopen("input.txt", "r");
    if(readFile == NULL)
    {
        printf("Unable to read input file");
        return 0;
    }

    int num_Hands = 0;
    Hand *Hand_DynamicArray = ReadHands(readFile, &num_Hands);

    SortArray(Hand_DynamicArray, 0, num_Hands-1);


    long int sum = 0;
    for(int i = 0; i<num_Hands; i++)
    {
        sum += (i+1) * Hand_DynamicArray[i].bet;
    }
    printf("%ld\n\n", sum);
    printf("NUM: %d\n", num_Hands);

    fclose(readFile);
}


/// @brief Takes in input file readPtr, int* to store number of cards counted. 
/// @return Stores each input into the Hand struct, and return a dynamic array containing those structs.
Hand* ReadHands(FILE* readPtr, int *counter)
{
    *counter = 0; 
    Hand *dynamicArr = malloc(sizeof(Hand));
    int current_AllocSize = sizeof(Hand);
    char buffer[500] = {0};
    
    //read and store input into Hand dynamic array.
    while(fgets(buffer, 500, readPtr)!=NULL)
    {
        ++*counter;
        if((size_t)current_AllocSize<*counter*sizeof(Hand))
        {
            dynamicArr = realloc(dynamicArr, current_AllocSize*2);
            current_AllocSize*=2;
        }
        
        sscanf(buffer, "%s %d", dynamicArr[*counter-1].cards, &dynamicArr[*counter-1].bet);
        dynamicArr[*counter-1].handType = CheckType(dynamicArr[*counter-1].cards);
    }

    return dynamicArr;
}

//Takes in string of 5 card characters, returns type of hand as type (enum).
type CheckType(const char *card)
{   
    char buffer[5];
    for(int i = 0; i<5; i++)
    {
        buffer[i] = card[i];
    }
    char temp;
    int matching[5] = {0};

    //count the number of matching cards.
    for(int i = 0; i<5; i++)
    {
        if(buffer[i] == '\0') continue; //don't check removed cards.
        temp = buffer[i];
        for(int j = i; j<5; j++) //check starting from and including current card's position, since it's guaranteed to have no matching cards before.
        {
            if(temp != buffer[j]) continue;
            buffer[j] = '\0'; //remove matching cards to prevent double count.
            matching[i]++;
        }
    }

    /*Theory: maximum and minimum of 5 cards in matching array.
    High Card --> No '0'.
    OnePair --> There's 1 '0'.
    TwoPair --> There's 2 '0's.
    ThreeKind --> There's 2 0's, and a 3.
    FullHouse --> There's 3 0's.
    FourKind --> There's a '4'.
    FiveKind --> There's a '5'.
    */

   int highest = 0, num_Zero = 0;
   for(int i = 0; i<5; i++)
   {
        if(matching[i] == 0)
        {
            num_Zero++;
            continue;
        }
        highest = (highest<matching[i])? matching[i]:highest;
   }

   switch(num_Zero)
   {
        case 0: return HIGHCARD;
        case 1: return ONEPAIR;
        case 2: 
            return (highest == 3)? THREEKIND:TWOPAIR;
   }
   switch(highest)
   {
        case 3: return FULLHOUSE; //since a 3kind would be returned earlier on.
        case 4: return FOURKIND;
        case 5: return FIVEKIND;
   }
    printf("Failure to assign \"%s\": Num_Zero: %d, Highesty: %d\n", card, num_Zero, highest);
    return NONE;
}

void SortArray(Hand *handArr, int lowIndex, int highIndex)
{
    //Sort by handType, then by the biggest card at the front.
    //Do it in ascending order. 
    if(lowIndex>=highIndex) return; //no need to sort an array of size 0-1. Base case.
    int swappedIndex = lowIndex - 1;
    Hand pivot = handArr[highIndex];

    for(int i = lowIndex; i<highIndex; i++) //don't check the pivot.
    {
        if(CompareHand(handArr[i], pivot)<0)
        {
            swappedIndex++; //since swappedIndex is pointing at the last swapped element, increment to the next (unswapped) element and swap.
            Swap(&handArr[i], &handArr[swappedIndex]);
        }
    }
    Swap(&handArr[highIndex], &handArr[swappedIndex+1]); //swaps pivot such that it is to the right of the swapped index.

    SortArray(handArr, lowIndex, swappedIndex+1-1);
    SortArray(handArr, swappedIndex+1+1, highIndex);
}

//return -ve if left is lower, 0 if equal, +ve if higher.
int CompareHand(Hand left, Hand right)
{
    char compareValue[] = {'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'};
    if(left.handType!=right.handType)
    {
        return (int)left.handType-(int)right.handType;
    }
    for(int i = 0; i<5; i++)
    {
        if(left.cards[i] == right.cards[i]) continue;
        //characters not equal, check against array.
        for(size_t j = 0; j<sizeof(compareValue)/sizeof(compareValue[0]); j++)
        {
            //the character that hits first is the bigger one.
            if(left.cards[i] == compareValue[j]) return 1;
            if(right.cards[i] == compareValue[j]) return -1;
        }
    }    
    return 0;
}

void Swap(Hand *left, Hand *right)
{
    Hand temp = *right;
    *right = *left;
    *left = temp;
}