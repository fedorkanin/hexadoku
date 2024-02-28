#pragma once

#include <stdlib.h>

typedef struct node {
    struct node* left;
    struct node* right;
    struct node* up;
    struct node* down;
    struct node* column_header;

    int          row_ID;  // if -1, then it's a column header
    int          column_ID;
    int          nodeCount;
} Node;

/// @brief Initializes a node with neighbor pointers pointing to itself, column
/// header set to NULL, and nodeCount set to 0.
/// @param rowID Row index of the node.
/// @param columnID Column index of the node.
/// @return A pointer to the newly created node.
Node* initNode(int rowID, int columnID);
