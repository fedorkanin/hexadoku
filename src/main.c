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

// ==================== EXACT COVER SOLVER ====================
// exact cover solver based on DLX algorithm by Donad Knuth
// https://arxiv.org/pdf/cs/0011047.pdf
bool compareNodeCount(Node* first, Node* second) {
    return first->nodeCount < second->nodeCount;
}

bool randomComparator(Node* first, Node* second) { return rand() % 2; }

// sort column headers by node count
void sortColumnHeaders(Node* head) {
    // de-cycle column headers
    head->left->right = NULL;

    head->right = mergeSort(head->right, compareNodeCount);
    // head->right = mergeSort(head->right, randomComparator);
    head->right->left = head;

    // find last column header
    Node* last_column_header = head->right;
    while (last_column_header->right)
        last_column_header = last_column_header->right;

    // re-cycle column headers
    head->left                = last_column_header;
    last_column_header->right = head;
}

// main solve function and helper functions inspired by
// https://www.geeksforgeeks.org/implementation-of-exact-cover-problem-and-algorithm-x-using-dlx/
// cover column
void cover(Node* column_header) {
    // unlink column header from horizontal neighbors
    column_header->left->right = column_header->right;
    column_header->right->left = column_header->left;
    // iterate over column
    for (Node* node = column_header->down; node != column_header;
         node       = node->down) {
        // iterate over row
        for (Node* row_node = node->right; row_node != node;
             row_node       = row_node->right) {
            // unlink row node from vertical neighbors
            row_node->up->down = row_node->down;
            row_node->down->up = row_node->up;
            // decrement node count of column header
            row_node->column_header->nodeCount--;
        }
    }
}

// uncover column
void uncover(Node* column_header) {
    // iterate over column
    for (Node* node = column_header->up; node != column_header;
         node       = node->up) {
        // iterate over row
        for (Node* row_node = node->left; row_node != node;
             row_node       = row_node->left) {
            // link row node with vertical neighbors
            row_node->up->down = row_node;
            row_node->down->up = row_node;
            // increment node count of column header
            row_node->column_header->nodeCount++;
        }
    }
    // link column header with horizontal neighbors
    column_header->left->right = column_header;
    column_header->right->left = column_header;
}

// get column with minimum node count
Node* getMinColumn(Node* head) {
    Node* min_column    = head->right;
    Node* column_header = head->right->right;
    while (column_header != head) {
        // if node count of a node is 1 or less, return that node, it can't be
        // any smaller
        if (column_header->nodeCount <= 1) return column_header;
        if (column_header->nodeCount < min_column->nodeCount)
            min_column = column_header;
        column_header = column_header->right;
    }
    return min_column;
}

uint8_t**  hexadoku_global;
int        solution_count_global = 0;
IntVector* solution_global;

void       solutionToHexadoku(IntVector* solution, uint8_t** hexadoku) {
    // DEBUG_PRINTF("In solutionToHexadoku\n");
    for (int i = 0; i < solution->size; i++) {
        int row    = rowFromExactCoverIndex(solution->data[i]);
        int column = columnFromExactCoverIndex(solution->data[i]);
        int value  = valueFromExactCoverIndex(solution->data[i]);
        hexadoku[row][column] = value;
    }
}

// print all solutions of monkey fist mesh
void searchSolutions(Node* head, int k) {
    Node* row_node;
    Node* right_node;
    Node* left_node;
    Node* column;

    // if there are no more columns, we found a solution
    if (head->right == head) {
        // convert solution to hexadoku
        if (solution_count_global == 0)
            solutionToHexadoku(solution_global, hexadoku_global);
        solution_count_global++;
        return;
    }
    // cover column with minimum node count
    column = getMinColumn(head);
    cover(column);
    // iterate over column
    for (row_node = column->down; row_node != column;
         row_node = row_node->down) {
        if (solution_count_global == 0)
            pushToIntVector(solution_global, row_node->row_ID);

        for (right_node = row_node->right; right_node != row_node;
             right_node = right_node->right)
            cover(right_node->column_header);
        // search for solutions
        searchSolutions(head, k + 1);
        // if solution is not possible, backtrack and uncover column
        if (solution_count_global == 0) popFromIntVector(solution_global);
        // column = row_node->column_header;
        for (left_node = row_node->left; left_node != row_node;
             left_node = left_node->left)
            uncover(left_node->column_header);
    }
    uncover(column);
    return;
}

int main(void) {
// measure time taken by each function
#ifdef MEASURE_TIME
    clock_t start, end;
    double  cpu_time_used;
    start = clock();
#endif

    printf("Zadejte hexadoku:\n");
    uint8_t** hexadoku = readIn();
    if (hexadoku == NULL) {
        printf("Nespravny vstup.\n");
        return 1;
    }
    if (!isHexadokuValid(hexadoku)) {
        printf("Nespravny vstup.\n");
        // free memory
        for (int i = 0; i < SUDOKU_SIZE; i++) free(hexadoku[i]);
        free(hexadoku);
        return 1;
    }

#ifdef MEASURE_TIME
    end           = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("readIn: %f\n", cpu_time_used);
#endif

    hexadoku_global = hexadoku;

#ifdef MEASURE_TIME
    start = clock();
#endif

    Node* head = createMonkeyFistMesh3(hexadoku);
    // sortColumnHeaders(head);

#ifdef MEASURE_TIME
    end           = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("createMonkeyFistMesh: %f\n", cpu_time_used);
#endif

#ifdef MEASURE_TIME
    start = clock();
#endif

    IntVector* solution = createIntVector(0);
    solution_global     = solution;
    searchSolutions(head, 0);

#ifdef MEASURE_TIME
    end           = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("searchSolutions: %f\n", cpu_time_used);
#endif

#ifndef MEASURE_TIME
    if (solution_count_global == 0) {
        printf("Reseni neexistuje.\n");
    } else if (solution_count_global == 1) {
        printHexadoku(hexadoku);
    } else {
        printf("Celkem reseni: %d\n", solution_count_global);
    }
#endif

// free memory
#ifdef MEASURE_TIME
    start = clock();
#endif
    for (int i = 0; i < SUDOKU_SIZE; i++) free(hexadoku[i]);
    free(hexadoku);
    freeMonkeyFistMesh(head);
    freeIntVector(solution);
#ifdef MEASURE_TIME
    end           = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("free memory: %f\n", cpu_time_used);
#endif
    return 0;
}
