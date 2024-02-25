#include "Solver.h"

// ==================== EXACT COVER SOLVER ====================
// exact cover solver based on DLX algorithm by Donad Knuth
// https://arxiv.org/pdf/cs/0011047.pdf
bool compareNodeCount(Node* first, Node* second) {
    return first->nodeCount < second->nodeCount;
}

// sort column headers by node count
void sortColumnHeaders(Node* head) {
    // de-cycle column headers
    head->left->right = NULL;

    head->right       = mergeSort(head->right, compareNodeCount);
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
