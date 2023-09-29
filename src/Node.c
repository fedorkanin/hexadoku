#include "Node.h"

Node* initNode(int rowID, int columnID) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        // Handle memory allocation error here, e.g., return NULL or exit the
        // program.
        return NULL;
    }

    node->left          = node;
    node->right         = node;
    node->up            = node;
    node->down          = node;
    node->column_header = NULL;
    node->row_ID        = rowID;
    node->column_ID     = columnID;
    node->nodeCount     = 0;
    return node;
}
