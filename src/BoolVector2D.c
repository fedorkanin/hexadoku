#include "BoolVector2D.h"

#include <stdio.h>
#include <stdlib.h>

BoolVector2D* createBoolVector2D(int capacity) {
    BoolVector2D* vector = (BoolVector2D*)malloc(sizeof(BoolVector2D));
    vector->data         = (BoolVector**)malloc(capacity * sizeof(BoolVector*));
    vector->size         = 0;
    vector->capacity     = capacity;
    return vector;
}

void pushToBoolVector2D(BoolVector2D* v, BoolVector* value) {
    if (v->size == v->capacity) {
        v->capacity = v->capacity == 0 ? 1 : 2 * v->capacity;
        v->data =
            (BoolVector**)realloc(v->data, v->capacity * sizeof(BoolVector*));
    }
    v->data[v->size++] = value;
}

void printBoolVector2D(BoolVector2D* v) {
    for (int i = 0; i < v->size; i++) {
        for (int j = 0; j < v->data[i]->size; j++) {
            printf("%d ", v->data[i]->data[j]);
        }
        printf("\n");
    }
}

BoolVector2D* boolArrayToVector2D(bool array[][4], int height, int width) {
    BoolVector2D* vector = createBoolVector2D(height);
    for (int i = 0; i < height; i++) {
        BoolVector* row = createBoolVector(width);
        for (int j = 0; j < width; j++) {
            pushToBoolVector(row, array[i][j]);
        }
        pushToBoolVector2D(vector, row);
    }
    return vector;
}

void freeBoolVector2D(BoolVector2D* v) {
    for (int i = 0; i < v->size; i++) freeBoolVector(v->data[i]);
    free(v->data);
    free(v);
}
