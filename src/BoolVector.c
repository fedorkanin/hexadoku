#include "BoolVector.h"

#include <stdio.h>
#include <stdlib.h>

BoolVector* createBoolVector(int capacity) {
    BoolVector* vector = (BoolVector*)malloc(sizeof(BoolVector));
    vector->data       = (bool*)malloc(capacity * sizeof(bool));
    vector->size       = 0;
    vector->capacity   = capacity;
    vector->is_active  = true;
    return vector;
}

void pushToBoolVector(BoolVector* v, bool value) {
    if (v->size == v->capacity) {
        v->capacity = v->capacity == 0 ? 1 : 2 * v->capacity;
        v->data     = (bool*)realloc(v->data, v->capacity * sizeof(bool));
    }
    v->data[v->size++] = value;
}

void popFromBoolVector(BoolVector* v) {
    if (v->size > 0) v->size--;
}

void fillBoolVector(BoolVector* v, bool value) {
    for (int i = 0; i < v->size; i++) {
        v->data[i] = value;
    }
}

void freeBoolVector(BoolVector* v) {
    free(v->data);
    free(v);
}