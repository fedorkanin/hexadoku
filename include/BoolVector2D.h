#pragma once

#include "BoolVector.h"

typedef struct BoolVector2D {
    BoolVector** data;
    int          size;
    int          capacity;
} BoolVector2D;

BoolVector2D* createBoolVector2D(int capacity);
void          pushToBoolVector2D(BoolVector2D* v, BoolVector* value);
void          printBoolVector2D(BoolVector2D* v);
void          freeBoolVector2D(BoolVector2D* v);
