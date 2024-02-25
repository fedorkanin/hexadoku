#include "MonkeyFistMesh.h"

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
