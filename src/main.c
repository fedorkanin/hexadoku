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

// create monkey fist mesh without using exact cover matrix
Node* createMonkeyFistMesh3(uint8_t** hexadoku) {
    DEBUG_PRINTF("In function createMonkeyFistMesh3()\n");

    int    mesh_width = SUDOKU_SIZE * SUDOKU_SIZE * 4;
    // int mesh_height = SUDOKU_SIZE * SUDOKU_SIZE * SUDOKU_SIZE;

    // create head node
    Node*  head = initNode(-1, -1);

    // create column nodes
    // create column headers array
    Node** column_headers = (Node**)malloc(mesh_width * sizeof(Node*));
    for (int i = 0; i < mesh_width; i++) {
        Node* column_node = initNode(-1, i);
        // link with left neighbor, link left neighbor with this node
        column_node->left        = i ? column_headers[i - 1] : head;
        column_node->left->right = column_node;
        // set column pointer
        column_node->column_header = column_node;
        // set node count
        column_node->nodeCount = 0;

        // set up and down pointers (try to delete later)
        column_node->up   = column_node;
        column_node->down = column_node;

        // add to column headers array
        column_headers[i] = column_node;
    }
    // link last column node with head node, first one is already linked
    column_headers[mesh_width - 1]->right = head;
    head->left                            = column_headers[mesh_width - 1];

    // create mesh nodes using pregenerated exact cover matrix
    // create first node manually
    // find first node allowed by clues
    int pregen_ind = 0;

    for (pregen_ind = 0; pregen_ind < COORDS_ARRAY_SIZE; pregen_ind++) {
        int hex_row_index =
            COORDS_ARRAY[pregen_ind][0] / (SUDOKU_SIZE * SUDOKU_SIZE);
        int hex_col_index =
            COORDS_ARRAY[pregen_ind][0] / SUDOKU_SIZE % SUDOKU_SIZE;
        int digit = COORDS_ARRAY[pregen_ind][0] % SUDOKU_SIZE + 1;
        // printf("hex_row_index = %d, hex_col_index = %d, digit = %d\n",
        //        hex_row_index, hex_col_index, digit);

        if (hexadoku[hex_row_index][hex_col_index] == 0 ||
            hexadoku[hex_row_index][hex_col_index] == digit)
            break;
    }

    Node* node =
        initNode(COORDS_ARRAY[pregen_ind][0], COORDS_ARRAY[pregen_ind][1]);
    node->left                = node;
    node->right               = node;
    node->column_header       = column_headers[node->column_ID];
    node->up                  = column_headers[node->column_ID];
    node->down                = column_headers[node->column_ID];
    node->column_header->up   = node;
    node->column_header->down = node;
    node->column_header->nodeCount++;
    Node* prev_node         = node;
    Node* first_node_in_row = node;
    pregen_ind++;

    for (; pregen_ind < COORDS_ARRAY_SIZE; pregen_ind++) {
        // check that this node is not forbidden by clues
        // find corresponding hexadoku cell
        int hex_row_index =
            COORDS_ARRAY[pregen_ind][0] / (SUDOKU_SIZE * SUDOKU_SIZE);
        int hex_col_index =
            COORDS_ARRAY[pregen_ind][0] / SUDOKU_SIZE % SUDOKU_SIZE;
        int digit = COORDS_ARRAY[pregen_ind][0] % SUDOKU_SIZE + 1;
        // printf("hex_row_index = %d, hex_col_index = %d, digit = %d\n",
        //        hex_row_index, hex_col_index, digit);

        if (hexadoku[hex_row_index][hex_col_index] != 0 &&
            hexadoku[hex_row_index][hex_col_index] != digit)
            continue;

        // create node
        int   row_index = COORDS_ARRAY[pregen_ind][0];
        int   col_index = COORDS_ARRAY[pregen_ind][1];

        Node* node = initNode(row_index, col_index);

        // link node with left neighbor symmetrically
        if (prev_node->row_ID == row_index) {
            node->left       = prev_node;
            prev_node->right = node;
        } else {
            // this node is first node in row
            node->left  = node;
            node->right = node;
            // link last and first nodes in previous row
            prev_node->right        = first_node_in_row;
            first_node_in_row->left = prev_node;
            // update first node in row
            first_node_in_row = node;
        }

        // link node with top neighbor symmetrically
        Node* last_node_in_column = column_headers[col_index]->up;
        node->up                  = last_node_in_column;
        last_node_in_column->down = node;
        // update column header
        node->column_header     = column_headers[col_index];
        node->column_header->up = node;
        node->down              = node->column_header;

        // increment node count of column header
        node->column_header->nodeCount++;

        // update pervious row node
        prev_node = node;
    }
    // link first and last nodes in last row
    prev_node->right        = first_node_in_row;
    first_node_in_row->left = prev_node;

    // free column headers array
    free(column_headers);

    return head;
}

// print monkey fist mesh
void printMonkeyFistMesh(Node* head) {
    Node* column_header = head->right;
    Node* node;
    while (column_header != head) {
        printf("Column %d:\n", column_header->column_ID);
        node = column_header->down;
        while (node != column_header) {
            printf("Row %d -> ", node->row_ID);
            printf("Left %d, Right %d, Up %d, Down %d\n", node->left->column_ID,
                   node->right->column_ID, node->up->row_ID,
                   node->down->row_ID);
            node = node->down;
        }
        column_header = column_header->right;
    }
}

// check if all nodes have non-null pointers
void validateMonkeyFistMesh(Node* head) {
    Node* column_header = head->right;
    Node* node;
    while (column_header != head) {
        if (column_header->nodeCount == 0) {
            printf("Column %d has no nodes\n", column_header->column_ID);
        }
        node = column_header->down;
        while (node != column_header) {
            if (node->left == NULL)
                printf("Node r%d, c%d has NULL left pointer\n", node->row_ID,
                       node->column_ID);
            if (node->right == NULL)
                printf("Node r%d, c%d has NULL right pointer\n", node->row_ID,
                       node->column_ID);
            if (node->up == NULL)
                printf("Node r%d, c%d has NULL up pointer\n", node->row_ID,
                       node->column_ID);
            if (node->down == NULL)
                printf("Node r%d, c%d has NULL down pointer \n", node->row_ID,
                       node->column_ID);
            if (node->column_header == NULL)
                printf("Node r%d, c%d has NULL column header pointer \n",
                       node->row_ID, node->column_ID);
            node = node->down;
        }
        column_header = column_header->right;
    }
    printf("Validation complete\n");
}

// free monkey fist mesh
void freeMonkeyFistMesh(Node* head) {
    Node* column_header = head->right;
    Node* node;
    while (column_header != head) {
        node = column_header->down;
        while (node != column_header) {
            Node* next_node = node->down;
            // DEBUG_PRINTF("Freeing node r%d, c%d\n", node->row_ID,
            //              node->column_ID);
            free(node);
            node = next_node;
        }
        Node* next_column_header = column_header->right;
        // DEBUG_PRINTF("Freeing column header c%d\n",
        // column_header->column_ID);
        free(column_header);
        column_header = next_column_header;
    }
    // DEBUG_PRINTF("Freeing head\n");
    free(head);
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
