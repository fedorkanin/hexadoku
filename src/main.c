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
#include "MonkeyFistMesh.h"
#include "Node.h"
#include "Solver.h"

uint8_t**  hexadoku_global;
int        solution_count_global = 0;
IntVector* solution_global;

void       solutionToHexadoku(IntVector* solution, uint8_t** hexadoku) {
    for (int i = 0; i < solution->size; i++) {
        int row    = rowFromExactCoverIndex(solution->data[i]);
        int column = columnFromExactCoverIndex(solution->data[i]);
        int value  = numFromExactCoverIndex(solution->data[i]);
        hexadoku[row][column] = value;
    }
}

void searchSolutions(Node* head, int k) {
    Node* row_node;
    Node* right_node;
    Node* left_node;
    Node* column;

    // If there are no more columns, we have found a solution.
    if (head->right == head) {
        if (solution_count_global == 0)
            solutionToHexadoku(solution_global, hexadoku_global);
        solution_count_global++;
        return;
    }

    // sortColumnHeaders(head);
    column = getMinColumn(head);
    cover(column);

    for (row_node = column->down; row_node != column;
         row_node = row_node->down) {
        if (solution_count_global == 0)
            pushToIntVector(solution_global, row_node->row_ID);

        for (right_node = row_node->right; right_node != row_node;
             right_node = right_node->right)
            cover(right_node->column_header);

        searchSolutions(head, k + 1);

        // if solution is not possible, backtrack and uncover column
        if (solution_count_global == 0) popFromIntVector(solution_global);

        for (left_node = row_node->left; left_node != row_node;
             left_node = left_node->left)
            uncover(left_node->column_header);
    }

    uncover(column);
    return;
}

int main(void) {
    printf("Zadejte hexadoku:\n");
    uint8_t** hexadoku = readProgtest();
    if (hexadoku == NULL) {
        printf("Nespravny vstup.\n");
        return 1;
    }
    if (!isHexadokuValid(hexadoku)) {
        printf("Nespravny vstup.\n");
        for (int i = 0; i < SUDOKU_SIZE; i++) free(hexadoku[i]);
        free(hexadoku);
        return 1;
    }

    hexadoku_global = hexadoku;

    Node*      head = createDLXMesh(hexadoku);

    IntVector* solution = createIntVector(0);
    solution_global     = solution;
    searchSolutions(head, 0);

    if (solution_count_global == 0) {
        printf("Reseni neexistuje.\n");
    } else if (solution_count_global == 1) {
        printHexadoku(hexadoku);
    } else {
        printf("Celkem reseni: %d\n", solution_count_global);
    }

    // free memory
    for (int i = 0; i < SUDOKU_SIZE; i++) free(hexadoku[i]);
    free(hexadoku);
    freeDLXMesh(head);
    freeIntVector(solution);

    return 0;
}
