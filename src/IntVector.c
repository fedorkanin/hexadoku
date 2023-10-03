#include "IntVector.h"

#include <stdio.h>
#include <stdlib.h>

IntVector* createIntVector(int capacity) {
    IntVector* vector = (IntVector*)malloc(sizeof(IntVector));
    vector->data      = (int*)malloc(capacity * sizeof(int));
    vector->size      = 0;
    vector->capacity  = capacity;
    return vector;
}

void pushToIntVector(IntVector* v, int value) {
    if (v->size == v->capacity) {
        v->capacity = v->capacity == 0 ? 1 : 2 * v->capacity;
        v->data     = (int*)realloc(v->data, v->capacity * sizeof(int));
    }
    v->data[v->size++] = value;
}

void popFromIntVector(IntVector* v) {
    if (v->size > 0) {
        v->size--;
    }
}

void printIntVector(IntVector* v) {
    for (int i = 0; i < v->size; i++) {
        printf("%d ", v->data[i]);
    }
    printf("\n");
}

void freeIntVector(IntVector* v) {
    if (v == NULL) return;
    free(v->data);
    free(v);
}
