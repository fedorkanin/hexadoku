#ifndef BOOLVECTOR2D_H
#define BOOLVECTOR2D_H

#include "BoolVector.h"

typedef struct BoolVector2D {
    BoolVector** data;
    int          size;
    int          capacity;
} BoolVector2D;

BoolVector2D* createBoolVector2D(int capacity);
void          pushToBoolVector2D(BoolVector2D* v, BoolVector* value);
void          printBoolVector2D(BoolVector2D* v);
BoolVector2D* boolArrayToVector2D(bool array[][4], int height, int width);
void          freeBoolVector2D(BoolVector2D* v);

#endif /* BOOLVECTOR2D_H */
