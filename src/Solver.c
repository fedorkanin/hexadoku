#include "Solver.h"

// main solve function and helper functions inspired by
// https://www.geeksforgeeks.org/implementation-of-exact-cover-problem-and-algorithm-x-using-dlx/

// Mark constraint as covered
//
// cover iterates over all rows that can no longer be used, because they contain
// a node in the already satisfied column. It detaches all the nodes in such
// rows from their vertical neighbors and decrements the node count of their
// respective column header.
void cover(Node* column_header) {
    unlinkHorizontal(column_header);

    for (Node* v_node = column_header->down; v_node != column_header;
         v_node       = v_node->down) {
        for (Node* h_node = v_node->right; h_node != v_node;
             h_node       = h_node->right) {
            unlinkVertical(h_node);
            h_node->column_header->nodeCount--;
        }
    }
}

// Uncover constraint, i.e. make it available again
//
// similar to cover, but in reverse
void uncover(Node* column_header) {
    for (Node* v_node = column_header->up; v_node != column_header;
         v_node       = v_node->up) {
        for (Node* h_node = v_node->left; h_node != v_node;
             h_node       = h_node->left) {
            relinkVertical(h_node);
            h_node->column_header->nodeCount++;
        }
    }

    relinkHorizontal(column_header);
}

// Get the column with minimum node count.
Node* getMinColumn(Node* head) {
    Node* min_column    = head->right;
    Node* column_header = head->right->right;
    while (column_header != head) {
        // Node count can't be any lower than 1.
        if (column_header->nodeCount <= 1) return column_header;
        if (column_header->nodeCount < min_column->nodeCount)
            min_column = column_header;
        column_header = column_header->right;
    }
    return min_column;
}

inline void unlinkHorizontal(Node* node) {
    node->left->right = node->right;
    node->right->left = node->left;
}

inline void relinkHorizontal(Node* node) {
    node->left->right = node;
    node->right->left = node;
}

inline void unlinkVertical(Node* node) {
    node->up->down = node->down;
    node->down->up = node->up;
}

inline void relinkVertical(Node* node) {
    node->up->down = node;
    node->down->up = node;
}
