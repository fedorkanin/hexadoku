#ifndef NODE_H
#define NODE_H

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

Node* initNode(int rowID, int columnID);

#endif  // NODE_H
