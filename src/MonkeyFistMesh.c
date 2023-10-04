#include "MonkeyFistMesh.h"

Node* createNodeFromData(int row_index, int col_index, Node** column_headers) {
    Node* node              = initNode(row_index, col_index);
    node->column_header     = column_headers[col_index];
    node->up                = column_headers[col_index]->up;
    node->down              = column_headers[col_index];
    node->column_header->up = node;
    node->column_header->nodeCount++;
    return node;
}

void linkNodesHorizontally(Node* left_node, Node* right_node) {
    left_node->right = right_node;
    right_node->left = left_node;
}

void linkNodesVertically(Node* upper_node, Node* lower_node) {
    upper_node->down = lower_node;
    lower_node->up   = upper_node;
}

Node** createColumnHeaders(int mesh_width, Node* head) {
    Node** column_headers = (Node**)malloc(mesh_width * sizeof(Node*));
    for (int i = 0; i < mesh_width; i++) {
        Node* column_node = initNode(-1, i);
        linkNodesHorizontally(i ? column_headers[i - 1] : head, column_node);
        column_node->column_header = column_node;
        column_node->nodeCount     = 0;
        linkNodesVertically(column_node, column_node);
        column_headers[i] = column_node;
    }
    linkNodesHorizontally(column_headers[mesh_width - 1], head);
    return column_headers;
}

void createNodesUsingPregenArray(uint8_t** cell_matrix, Node** column_headers) {
    int pregen_ind;
    for (pregen_ind = 0; pregen_ind < COORDS_ARRAY_SIZE; pregen_ind++) {
        int sudoku_row    = sudokuRowFromExactCoverRow(pregen_ind);
        int sudoku_column = sudokuColumnFromExactCoverRow(pregen_ind);
        int sudoku_value  = sudokuValueFromExactCoverRow(pregen_ind);

        if (cell_matrix[sudoku_row][sudoku_column] == 0 ||
            cell_matrix[sudoku_row][sudoku_column] == sudoku_value)
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
        int hex_row_index =
            COORDS_ARRAY[pregen_ind][0] / (SUDOKU_SIZE * SUDOKU_SIZE);
        int hex_col_index =
            COORDS_ARRAY[pregen_ind][0] / SUDOKU_SIZE % SUDOKU_SIZE;
        int digit = COORDS_ARRAY[pregen_ind][0] % SUDOKU_SIZE + 1;

        if (cell_matrix[hex_row_index][hex_col_index] != 0 &&
            cell_matrix[hex_row_index][hex_col_index] != digit)
            continue;

        int   row_index = COORDS_ARRAY[pregen_ind][0];
        int   col_index = COORDS_ARRAY[pregen_ind][1];
        Node* node      = initNode(row_index, col_index);

        if (prev_node->row_ID == row_index) {
            node->left       = prev_node;
            prev_node->right = node;
        } else {
            node->left              = node;
            node->right             = node;
            prev_node->right        = first_node_in_row;
            first_node_in_row->left = prev_node;
            first_node_in_row       = node;
        }

        Node* last_node_in_column = column_headers[col_index]->up;
        node->up                  = last_node_in_column;
        last_node_in_column->down = node;
        node->column_header       = column_headers[col_index];
        node->column_header->up   = node;
        node->down                = node->column_header;
        node->column_header->nodeCount++;
        prev_node = node;
    }
    prev_node->right        = first_node_in_row;
    first_node_in_row->left = prev_node;
}

void createNodesUsingExactCover(uint8_t** cell_matrix, Node** column_headers) {
    BoolVector2D* exact_cover = hexadokuToExactCover(cell_matrix);

    Node*         prev_node         = NULL;
    Node*         first_node_in_row = NULL;

    for (int i = 0; i < exact_cover->capacity - 1; i++) {
        for (int j = 0; j < exact_cover->data[i]->capacity; j++) {
            if (exact_cover->data[i + 1]->data[j]) {
                Node* node = createNodeFromData(i, j, column_headers);
                if (prev_node && prev_node->row_ID == i) {
                    linkNodesHorizontally(prev_node, node);
                } else {
                    linkNodesHorizontally(node, node);
                    if (prev_node) {
                        linkNodesHorizontally(prev_node, first_node_in_row);
                    }
                    first_node_in_row = node;
                }
                linkNodesVertically(node->up, node);
                prev_node = node;
            }
        }
    }

    if (prev_node) {
        linkNodesHorizontally(prev_node, first_node_in_row);
    }
    freeBoolVector2D(exact_cover);
}

Node* createMonkeyFistMesh(uint8_t** cell_matrix) {
    DEBUG_PRINTF("In function createMonkeyFistMesh()\n");

    int    mesh_width     = SUDOKU_SIZE * SUDOKU_SIZE * 4;
    Node*  head           = initNode(-1, -1);
    Node** column_headers = createColumnHeaders(mesh_width, head);

#ifdef USE_PREGEN_ARRAY
    createNodesUsingPregenArray(cell_matrix, column_headers);
#else
    createNodesUsingExactCover(cell_matrix, column_headers);
#endif

    free(column_headers);
    return head;
}

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
            free(node);
            node = next_node;
        }
        Node* next_column_header = column_header->right;

        free(column_header);
        column_header = next_column_header;
    }

    free(head);
}
