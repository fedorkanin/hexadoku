#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "BoolVector.h"
#include "BoolVector2D.h"
#include "Coords.h"
#include "ExactCover.h"
#include "Hexadoku.h"
#include "InputFunctions.h"
#include "IntVector.h"
#include "MergeSort.h"
#include "MonkeyFistMesh.h"
#include "Node.h"
#include "Solver.h"

void solutionToHexadoku(IntVector* solution, Sudoku* hex) {
    for (int i = 0; i < solution->size; i++) {
        int row    = sudokuRowFromExactCoverRow(solution->data[i]);
        int column = sudokuColumnFromExactCoverRow(solution->data[i]);
        int value  = sudokuValueFromExactCoverRow(solution->data[i]);

        // Update the hexadoku within the Sudoku struct
        hex->cell_matrix[row][column] = value;
    }
}

void searchSolutions(Node* head, int k, Sudoku* hex) {
    Node* row_node;
    Node* right_node;
    Node* left_node;
    Node* column;

    if (head->right == head) {
        if (hex->solution_count == 0) solutionToHexadoku(hex->solution, hex);
        hex->solution_count++;
        return;
    }

    column = getMinColumn(head);
    cover(column);

    for (row_node = column->down; row_node != column;
         row_node = row_node->down) {
        if (hex->solution_count == 0)
            pushToIntVector(hex->solution, row_node->row_ID);

        for (right_node = row_node->right; right_node != row_node;
             right_node = right_node->right)
            cover(right_node->column_header);

        searchSolutions(head, k + 1, hex);

        if (hex->solution_count == 0) popFromIntVector(hex->solution);

        for (left_node = row_node->left; left_node != row_node;
             left_node = left_node->left)
            uncover(left_node->column_header);
    }
    uncover(column);
}

int main(void) {
    printf("Zadejte hexadoku:\n");

    Sudoku* hex = readIn();
    if (hex == NULL) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    if (!isHexadokuValid(hex)) {
        printf("Nespravny vstup.\n");
        freeHexadoku(hex);
        return 1;
    }

    hex->solution       = createIntVector(0);
    hex->solution_count = 0;

    Node* head = createMonkeyFistMesh(hex->cell_matrix);
    searchSolutions(head, 0, hex);

    if (hex->solution_count == 0) {
        printf("Reseni neexistuje.\n");
    } else if (hex->solution_count == 1) {
        printHexadoku(hex);
    } else {
        printf("Celkem reseni: %d\n", hex->solution_count);
    }

    freeHexadoku(hex);
    freeMonkeyFistMesh(head);

    return 0;
}
