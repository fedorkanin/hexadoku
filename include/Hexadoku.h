#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Constants.h"
#include "IntVector.h"

typedef struct {
    uint8_t**  cell_matrix;
    IntVector* solution;
    int        solution_count;
} Sudoku;

bool isHexadokuValid(Sudoku* hex);
void printSudoku(Sudoku* hex);
void printSudokuAsLine(Sudoku* hex);
void freeHexadoku(Sudoku* hex);
