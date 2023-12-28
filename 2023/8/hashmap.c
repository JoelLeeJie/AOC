#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"


int main(void)
{
    static Bucket hashMap[160] = {0};

    FILE* readPtr = fopen("input.txt", "r");
    if(readPtr == NULL)
    {
        printf("Unable to open input file");
        return 0;
    }

    char instructions[500];
    fgets(instructions, 500, readPtr);
    PutInputInHashMap(hashMap, 160, readPtr);

    printf("\n\n\n");
    for(int i = 0, counter = 0; i<160; i++)
    {
        Bucket *temp = hashMap+i;
        /*
        while(temp->nextPtr!=NULL)
        {
            printf("%d. %s: (%s, %s)   ", counter++, temp->keyPair.key, temp->keyPair.left, temp->keyPair.right);
            temp = temp->nextPtr;
        }
        */
        printf("%d. %s: (%s, %s)   ", counter++, temp->keyPair.key, temp->keyPair.left, temp->keyPair.right);
        printf("\n");
    }

    fclose(readPtr);
    ClearHashMap(hashMap, 160);
}

void PutInputInHashMap(Bucket *hashMapPtr, int length, FILE* readPtr)
{
    KeyValue temp;
    char buffer[500];
    while(fgets(buffer, 500, readPtr)!=NULL)
    {
        if(buffer[0] == '\n') continue;
        sscanf(buffer, "%s %*[^(](%[^,], %[^)]", temp.key, temp.left, temp.right);
        PutHashMap(temp, hashMapPtr, length);
    }
}


//returns an integer based on input
int GetHash(const char *input)
{
    int sum = 0;
    //run until '\0' is hit.
    for(int i = 0; *(input+i)!='\0'; i++)
    {
        sum += i * 7 * (int)(*(input+i));
    }
    return sum;
}

/// @brief 
/// @param keyPair is a struct containing the key and its corresponding values.
/// @param hashMapPtr is the base pointer to the array of linked lists 
/// @param length is the length of the hashmap array
void PutHashMap(KeyValue input, Bucket *hashMapPtr, int length)
{
    if(hashMapPtr == NULL) return;

    int hash = GetHash(input.key);
    hashMapPtr += hash%length;

    //check for empty space, if not then make a new one.
    while(hashMapPtr->keyPair.key[0] != '\0')
    {
        if(hashMapPtr->nextPtr != NULL) 
        {
            hashMapPtr = hashMapPtr->nextPtr; //traverse list to next element.
            continue;
        }
        //else create a new space in the list.
        hashMapPtr->nextPtr = malloc(sizeof(Bucket));
        hashMapPtr = hashMapPtr->nextPtr;
        memset(hashMapPtr, 0, sizeof(Bucket)); //set the element to zero.
    }
    hashMapPtr->keyPair = input;
}

KeyValue* ReadHashMap(const char* key, Bucket *hashMapPtr, int length)
{
    int hash = GetHash(key);
    hashMapPtr += hash%length;

    //traverse list until right key is found.
    while(CompareString(hashMapPtr->keyPair.key, key))
    {
        if(hashMapPtr->nextPtr == NULL) return NULL;
        hashMapPtr = hashMapPtr->nextPtr;
    }
    return &hashMapPtr->keyPair;
}

void ClearHashMap(Bucket *hashMapPtr, int length)
{
    Bucket *base = hashMapPtr;
    //run through array and iteratively free up each linked list.
    for(int i = 0; i<length; i++)
    {
        hashMapPtr = base+i;
        if(hashMapPtr->nextPtr == NULL) continue;
        hashMapPtr = hashMapPtr->nextPtr; //since the start of the list isn't in dynamic memory, but rather in an array.
        while(hashMapPtr->nextPtr != NULL)
        {
            Bucket *tmp = hashMapPtr;
            hashMapPtr = hashMapPtr->nextPtr;
            free(tmp);
        }
    }
}

//return 0 if both strings are equal.
int CompareString(const char* left, const char* right)
{
    //run if one string hasn't reached the end.
    for(int i = 0; *(left+i)!='\0' || *(right+i)!='\0'; i++)
    {
        if(*(left+i)=='\0') return -1;
        if(*(right+i)=='\0') return 1;

        if(*(left+i) == *(right+i)) continue;
        return *(left+i) - *(right+i);
    }
    return 0;
}