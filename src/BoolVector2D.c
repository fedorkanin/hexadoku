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

void freeBoolVector2D(BoolVector2D* v) {
    for (int i = 0; i < v->size; i++) freeBoolVector(v->data[i]);
    free(v->data);
    free(v);
}
