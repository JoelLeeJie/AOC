#include <stdio.h>
typedef struct KeyValue{
    char key[4];
    //values
    char left[4];
    char right[4];
}KeyValue;

typedef struct Bucket{
    KeyValue keyPair;
    struct Bucket *nextPtr;
}Bucket;

void PutInputInHashMap(Bucket *hashMapPtr, int length, FILE* readPtr);
int GetHash(const char *input);
void PutHashMap(KeyValue input, Bucket *hashMapPtr, int length);
KeyValue* ReadHashMap(const char* key, Bucket *hashMapPtr, int length);
void ClearHashMap(Bucket *hashMapPtr, int length);
int CompareString(const char* left, const char* right);
