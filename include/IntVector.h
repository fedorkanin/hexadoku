#pragma once

typedef struct IntVector {
    int* data;
    int  size;
    int  capacity;
} IntVector;

IntVector* createIntVector(int capacity);
void       pushToIntVector(IntVector* v, int value);
void       popFromIntVector(IntVector* v);
void       printIntVector(IntVector* v);
void       freeIntVector(IntVector* v);