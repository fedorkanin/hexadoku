#ifndef BOOLVECTOR_H
#define BOOLVECTOR_H

#include <stdbool.h>

typedef struct BoolVector {
    bool* data;
    int   size;
    int   capacity;
    bool  is_active;
} BoolVector;

BoolVector* createBoolVector(int capacity);
void        pushToBoolVector(BoolVector* v, bool value);
void        popFromBoolVector(BoolVector* v);
void        fillBoolVector(BoolVector* v, bool value);
void        freeBoolVector(BoolVector* v);

#endif /* BOOLVECTOR_H */
