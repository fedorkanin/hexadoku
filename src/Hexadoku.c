#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Coords.h"

#define SUDOKU_SIZE 16
#define BOX_SIZE 4
#define DEBUG 0

size_t LINE_WIDTH  = 4 * SUDOKU_SIZE + 1;
size_t LINE_HEIGHT = 2 * SUDOKU_SIZE + 1;

bool   isDelimiterStringValid(char* string, bool is_dashed) {
    size_t line_width = (int)SUDOKU_SIZE * 4 + 1;
    if (strlen(string) != line_width) {
        if (DEBUG) printf("Invalid line width.\n");
        if (DEBUG)
            printf("Expected: %zu, got: %zu\n", line_width, strlen(string));
        return false;
    }

    if (is_dashed) {
        for (size_t i = 0; i < line_width; i++) {
            if (i % 4 == 0 && string[i] != '+') {
                if (DEBUG) printf("Invalid character at position %zu.\n", i);
                if (DEBUG) printf("Expected: '+', got: '%c'\n", string[i]);
                return false;
            } else if (i % 4 && string[i] != ' ') {
                if (DEBUG) printf("Invalid character at position %zu.\n", i);
                if (DEBUG) printf("Expected: ' ', got: '%c'\n", string[i]);
                return false;
            }
        }
    } else {
        for (size_t i = 0; i < line_width; i++) {
            if (i % 4 == 0 && string[i] != '+') {
                if (DEBUG) printf("Invalid character at position %zu.\n", i);
                if (DEBUG) printf("Expected: '+', got: '%c'\n", string[i]);
                return false;
            } else if (i % 4 && string[i] != '-') {
                if (DEBUG) printf("Invalid character at position %zu.\n", i);
                if (DEBUG) printf("Expected: '-', got: '%c'\n", string[i]);
                return false;
            }
        }
    }
    return true;
}

bool isLetterValid(char a) {
    if (a >= 'a' && a <= 'p') return true;
    if (a == ' ') return true;
    return false;
}

// convert string to array of uint8_t (0-16)
uint8_t* strToUint8t(char* string) {
    if (strlen(string) != LINE_WIDTH) {
        if (DEBUG) printf("Invalid line width.\n");
        return NULL;
    }

    uint8_t* array = (uint8_t*)malloc(SUDOKU_SIZE * sizeof(uint8_t));
    for (size_t i = 0; i < LINE_WIDTH; i++) {
        if (i % 16 == 0 && string[i] == '|') {
            continue;
        } else if ((i - 2) % 4 == 0) {
            // letter posiition, check if valid,
            // convert to char and store
            if (isLetterValid(string[i]))
                array[(i - 2) / 4] =
                    (string[i] == ' ') ? 0 : string[i] - 'a' + 1;
            else {
                if (DEBUG) printf("Invalid letter at position %zu.\n", i);
                free(array);
                return NULL;
            }
        } else if (string[i] == ' ' && i % 16 != 0) {
            continue;
        } else {
            if (DEBUG) printf("Invalid character at position %zu.\n", i);
            free(array);
            return NULL;
        }
    }

    return array;
}

// remove all spaces from string end
void stripString(char* string) {
    int len = strlen(string);
    while (len > 0 && string[len - 1] == ' ') {
        string[len - 1] = '\0';
        len--;
    }
}

// read string from stdin using getline, returns '\0' string in case
// of failure
char* getString() {
    char*  string = NULL;
    size_t size   = 0;
    if (getline(&string, &size, stdin) == -1) {
        free(string);
        string    = (char*)malloc(sizeof(char));
        string[0] = '\0';
    }
    // remove trailing newline
    int len = strlen(string);
    if (len > 0 && string[len - 1] == '\n') string[len - 1] = '\0';
    stripString(string);

    return string;
}

// read 16x16 hexadoku from stdin. Unset positions are represented by 0.
uint8_t** readIn() {
    uint8_t** hexadoku = (uint8_t**)malloc(16 * sizeof(char*));
    // read first line
    char*     line = getString();
    if (!isDelimiterStringValid(line, false)) {
        if (DEBUG) printf("Invalid first line.\n");
        free(line);
        free(hexadoku);
        return NULL;
    }

    // read line with letters and delimiter lines
    for (size_t i = 0; i < LINE_HEIGHT - 2; i++) {
        free(line);
        if (i % 2 == 0) {
            // read line with letters
            line            = getString();
            hexadoku[i / 2] = strToUint8t(line);
            if (hexadoku[i / 2] == NULL) {
                if (DEBUG) printf("Invalid line %zu.\n", i / 2 + 1);
                free(line);
                for (size_t j = 0; j <= i / 2; j++) free(hexadoku[j]);
                free(hexadoku);
                return NULL;
            }
        } else {
            // read delimiter line
            line           = getString();
            bool is_dashed = (i / 2 + 1) % 4 == 0 ? false : true;
            if (!isDelimiterStringValid(line, is_dashed)) {
                if (DEBUG) printf("Invalid delimiter line %zu.\n", i / 2 + 1);
                free(line);
                for (size_t j = 0; j <= i / 2; j++) free(hexadoku[j]);
                free(hexadoku);
                return NULL;
            }
        }
    }
    free(line);

    // read last line
    line = getString();
    if (!isDelimiterStringValid(line, false)) {
        if (DEBUG) printf("Invalid last line.\n");
        free(line);
        for (int j = 0; j < 16; j++) free(hexadoku[j]);
        free(hexadoku);
        return NULL;
    }
    free(line);

    // check no characters are left in stdin
    line = getString();
    if (strlen(line) != 0) {
        if (DEBUG) printf("Input after hexadoku.\n");
        free(line);
        for (int j = 0; j < 16; j++) free(hexadoku[j]);
        free(hexadoku);
        return NULL;
    }

    return hexadoku;
}

// check if hexadoku is valid
bool isHexadokuValid(uint8_t** hexadoku) {
    // check rows
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        bool row[SUDOKU_SIZE] = {0};
        for (int j = 0; j < SUDOKU_SIZE; j++) {
            if (hexadoku[i][j] == 0) continue;
            if (row[hexadoku[i][j] - 1] != false) {
                if (DEBUG) printf("Invalid row %d.\n", i + 1);
                return false;
            }
            row[hexadoku[i][j] - 1] = true;
        }
    }

    // check columns
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        bool column[SUDOKU_SIZE] = {0};
        for (int j = 0; j < SUDOKU_SIZE; j++) {
            if (hexadoku[j][i] == 0) continue;
            if (column[hexadoku[j][i] - 1] != false) {
                if (DEBUG) printf("Invalid column %d.\n", i + 1);
                return false;
            }
            column[hexadoku[j][i] - 1] = true;
        }
    }

    // check blocks
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        bool block[SUDOKU_SIZE] = {0};
        for (int j = 0; j < SUDOKU_SIZE; j++) {
            int row    = i / BOX_SIZE * BOX_SIZE + j / BOX_SIZE;
            int column = i % BOX_SIZE * BOX_SIZE + j % BOX_SIZE;
            if (hexadoku[row][column] == 0) continue;
            if (block[hexadoku[row][column] - 1] != false) {
                if (DEBUG) printf("Invalid block %d.\n", i + 1);
                return false;
            }
            block[hexadoku[row][column] - 1] = true;
        }
    }

    return true;
}

// print 16x16 hexadoku
void printHexadoku(uint8_t** hexadoku) {
    // print first line
    for (size_t i = 0; i < LINE_WIDTH - 1; i++) {
        if (i % BOX_SIZE == 0) {
            printf("+");
        } else {
            printf("-");
        }
    }
    printf("+\n");
    for (size_t i = 0; i < LINE_HEIGHT - 1; i++) {
        if (i % 2 == 0) {
            // print line with letters
            for (size_t j = 0; j < LINE_WIDTH - 1; j++) {
                if (j % 16 == 0) {
                    printf("|");
                } else if ((j - 2) % 4 == 0) {
                    // letter position
                    char letter = hexadoku[i / 2][(j - 2) / 4];
                    printf("%c", letter == 0 ? ' ' : letter + 'a' - 1);
                } else {
                    printf(" ");
                }
            }
            printf("|\n");
        } else {
            // print delimiter line
            bool is_dashed = (i / 2 + 1) % 4 == 0 ? true : false;
            for (size_t j = 0; j < LINE_WIDTH; j++) {
                if (j % 4 == 0) {
                    printf("+");
                } else {
                    printf(is_dashed ? "-" : " ");
                }
            }
            printf("\n");
        }
    }
}

typedef struct IntVector {
    int* data;
    int  size;
    int  capacity;
} IntVector;

IntVector* createIntVector(int capacity) {
    IntVector* vector = (IntVector*)malloc(sizeof(IntVector));
    vector->data      = (int*)malloc(capacity * sizeof(int));
    vector->size      = 0;
    vector->capacity  = capacity;
    return vector;
}

void pushToIntVector(IntVector* v, int value) {
    if (v->size == v->capacity) {
        v->capacity = v->capacity == 0 ? 1 : 2 * v->capacity;
        v->data     = (int*)realloc(v->data, v->capacity * sizeof(int));
    }
    v->data[v->size++] = value;
}

void popFromIntVector(IntVector* v) {
    if (v->size > 0) {
        v->size--;
    }
}

void printIntVector(IntVector* v) {
    for (int i = 0; i < v->size; i++) {
        printf("%d ", v->data[i]);
    }
    printf("\n");
}

void freeIntVector(IntVector* v) {
    free(v->data);
    free(v);
}

typedef struct BoolVector {
    bool* data;
    int   size;
    int   capacity;
    bool  is_active;
} BoolVector;

BoolVector* createBoolVector(int capacity) {
    BoolVector* vector = (BoolVector*)malloc(sizeof(BoolVector));
    vector->data       = (bool*)malloc(capacity * sizeof(bool));
    vector->size       = 0;
    vector->capacity   = capacity;
    vector->is_active  = true;
    return vector;
}

void pushToBoolVector(BoolVector* v, bool value) {
    if (v->size == v->capacity) {
        v->capacity = v->capacity == 0 ? 1 : 2 * v->capacity;
        v->data     = (bool*)realloc(v->data, v->capacity * sizeof(bool));
    }
    v->data[v->size++] = value;
}

void popFromBoolVector(BoolVector* v) {
    if (v->size > 0) v->size--;
}

void fillBoolVector(BoolVector* v, bool value) {
    for (int i = 0; i < v->size; i++) {
        v->data[i] = value;
    }
}

void freeBoolVector(BoolVector* v) {
    free(v->data);
    free(v);
}

typedef struct BoolVector2D {
    BoolVector** data;
    int          size;
    int          capacity;
} BoolVector2D;

BoolVector2D* createBoolVector2D(int capacity) {
    BoolVector2D* vector = (BoolVector2D*)malloc(sizeof(BoolVector2D));
    vector->data         = (BoolVector**)malloc(capacity * sizeof(BoolVector*));
    vector->size         = 0;
    vector->capacity     = capacity;
    return vector;
}

void pushToBoolVector2D(BoolVector2D* v, BoolVector* value) {
    if (v->size == v->capacity) {
        v->capacity = v->capacity == 0 ? 1 : 2 * v->capacity;
        v->data =
            (BoolVector**)realloc(v->data, v->capacity * sizeof(BoolVector*));
    }
    v->data[v->size++] = value;
}

void printBoolVector2D(BoolVector2D* v) {
    for (int i = 0; i < v->size; i++) {
        for (int j = 0; j < v->data[i]->size; j++) {
            printf("%d ", v->data[i]->data[j]);
        }
        printf("\n");
    }
}

BoolVector2D* boolArrayToVector2D(bool array[][4], int height, int width) {
    BoolVector2D* vector = createBoolVector2D(height);
    for (int i = 0; i < height; i++) {
        BoolVector* row = createBoolVector(width);
        for (int j = 0; j < width; j++) {
            pushToBoolVector(row, array[i][j]);
        }
        pushToBoolVector2D(vector, row);
    }
    return vector;
}

void freeBoolVector2D(BoolVector2D* v) {
    for (int i = 0; i < v->size; i++) freeBoolVector(v->data[i]);
    free(v->data);
    free(v);
}

int indexInExactCoverMatrix(int row, int column, int num) {
    // + 1 because first row is header
    return row * SUDOKU_SIZE * SUDOKU_SIZE + column * SUDOKU_SIZE + num + 1;
}

int valueFromExactCoverIndex(int index) { return index % SUDOKU_SIZE + 1; }

int columnFromExactCoverIndex(int index) {
    return (index / SUDOKU_SIZE) % SUDOKU_SIZE;
}

int rowFromExactCoverIndex(int index) {
    return (index / SUDOKU_SIZE / SUDOKU_SIZE);
}

int createCellConstraints(BoolVector2D* exact_cover, int header) {
    for (int row = 0; row < SUDOKU_SIZE; row++)
        for (int column = 0; column < SUDOKU_SIZE; column++, header++)
            for (int num = 0; num < SUDOKU_SIZE; num++) {
                exact_cover->data[indexInExactCoverMatrix(row, column, num)]
                    ->data[header] = true;
            }
    return header;
}

int createRowConstraints(BoolVector2D* exact_cover, int header) {
    for (int row = 0; row < SUDOKU_SIZE; row++)
        for (int num = 0; num < SUDOKU_SIZE; num++, header++)
            for (int column = 0; column < SUDOKU_SIZE; column++) {
                exact_cover->data[indexInExactCoverMatrix(row, column, num)]
                    ->data[header] = true;
            }
    return header;
}

int createColumnConstraints(BoolVector2D* exact_cover, int header) {
    for (int column = 0; column < SUDOKU_SIZE; column++)
        for (int num = 0; num < SUDOKU_SIZE; num++, header++)
            for (int row = 0; row < SUDOKU_SIZE; row++) {
                exact_cover->data[indexInExactCoverMatrix(row, column, num)]
                    ->data[header] = true;
            }
    return header;
}

int createBoxConstraints(BoolVector2D* exact_cover, int header) {
    for (int row = 0; row < SUDOKU_SIZE; row += BOX_SIZE)
        for (int column = 0; column < SUDOKU_SIZE; column += BOX_SIZE)
            for (int num = 0; num < SUDOKU_SIZE; num++, header++)
                for (int box_row = 0; box_row < BOX_SIZE; box_row++)
                    for (int box_column = 0; box_column < BOX_SIZE;
                         box_column++) {
                        int index = indexInExactCoverMatrix(
                            row + box_row, column + box_column, num);
                        exact_cover->data[index]->data[header] = true;
                    }

    return header;
}

BoolVector2D* hexadokuToExactCover(uint8_t** hexadoku) {
    if (DEBUG) printf("Converting hexadoku to exact cover...\n");
    // possible candidates for each cell
    int           rows_number = pow(SUDOKU_SIZE, 3) + 1;
    // 4 constraints for each cell
    int           columns_number = pow(SUDOKU_SIZE, 2) * 4;
    // initialize exact cover matrix
    BoolVector2D* exact_cover = createBoolVector2D(rows_number);

    // push first row consisting of 1s (required for transformaing to monkey
    // fist)
    pushToBoolVector2D(exact_cover, createBoolVector(columns_number));
    exact_cover->data[0]->size = exact_cover->data[0]->capacity;
    for (int i = 0; i < columns_number; i++)
        exact_cover->data[0]->data[i] = true;

    // Push rows
    for (int i = 1; i < rows_number; i++) {
        pushToBoolVector2D(exact_cover, createBoolVector(columns_number));
        exact_cover->data[i]->size = exact_cover->data[i]->capacity;
    }
    // fill with zeros
    for (int i = 1; i < rows_number; i++)
        for (int j = 0; j < columns_number; j++)
            exact_cover->data[i]->data[j] = false;

    // print width and height of BoolVector2D
    if (DEBUG)
        printf("Matrix width: %d, height: %d\n", exact_cover->data[0]->capacity,
               exact_cover->capacity);

    // fill exact cover matrix as if sudoku was empty
    // use indexInExactCoverMatrix to get index in exact cover matrix
    // for each cell
    // inspired by java implementation:
    // https://medium.com/javarevisited/buiding-a-sudoku-solver-in-java-with-dancing-links-180274b0b6c1
    int header = 0;  // first row is already filled with 1s, so start from 1
    header     = createCellConstraints(exact_cover, header);
    header     = createRowConstraints(exact_cover, header);
    header     = createColumnConstraints(exact_cover, header);
    createBoxConstraints(exact_cover, header);

    // fill exact cover matrix with given sudoku clues
    for (int row = 0; row < SUDOKU_SIZE; row++) {
        for (int column = 0; column < SUDOKU_SIZE; column++) {
            int cur_clue = hexadoku[row][column];
            if (cur_clue != 0) {
                for (int num = 0; num < SUDOKU_SIZE; num++) {
                    if (num != cur_clue - 1) {
                        // fill with 0s
                        fillBoolVector(
                            exact_cover->data[indexInExactCoverMatrix(
                                row, column, num)],
                            false);
                    }
                }
            }
        }
    }
    return exact_cover;
}

// validate exact cover matrix
bool isExactCoverValid(BoolVector2D* exact_cover) {
    if (DEBUG) printf("Validating exact cover matrix...\n");
    // check that first row is filled with 1s
    for (int i = 0; i < exact_cover->data[0]->capacity; i++) {
        if (!exact_cover->data[0]->data[i]) {
            if (DEBUG) printf("First row is not filled with 1s!\n");
            return false;
        }
    }
    // check if each column has at least one 1
    for (int i = 0; i < exact_cover->data[0]->capacity; i++) {
        bool has_one = false;
        for (int j = 1; j < exact_cover->capacity; j++) {
            if (exact_cover->data[j]->data[i]) {
                has_one = true;
                break;
            }
        }
        if (!has_one) {
            if (DEBUG) printf("Column %d has no 1s!\n", i);
            return false;
        }
    }
    if (DEBUG) printf("Exact cover validation complete!\n");
    return true;
}

// ==================== EXACT COVER SOLVER ====================
// exact cover solver based on DLX algorithm by Donad Knuth
// https://arxiv.org/pdf/cs/0011047.pdf

// struct node
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

Node* initNode(int rowID, int columnID) {
    Node* node          = (Node*)malloc(sizeof(Node));
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

Node* createMonkeyFistMesh2(BoolVector2D* exact_cover) {
    if (DEBUG) printf("In function createMonkeyFistMesh2()\n");
    // create head node
    Node*  head = initNode(-1, -1);

    int    mesh_width  = exact_cover->data[0]->capacity;
    int    mesh_height = exact_cover->capacity;

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
        // add to column headers array
        column_headers[i] = column_node;
    }
    // link last column node with head node, first one is already linked
    column_headers[mesh_width - 1]->right = head;
    head->left                            = column_headers[mesh_width - 1];

    // create mesh nodes
    for (int row_index = 1; row_index < mesh_height; row_index++) {
        Node* node;
        Node* pervious_row_node = NULL;
        Node* first_node_in_row = NULL;
        for (int col_index = 0; col_index < mesh_width; col_index++) {
            if (exact_cover->data[row_index]->data[col_index] == false)
                continue;
            // create node
            node = initNode(row_index - 1, col_index);
            // link node with left neighbor symmetrically
            node->left = pervious_row_node;
            if (pervious_row_node) pervious_row_node->right = node;
            // link node with top neighbor symmetrically
            Node* last_node_in_column = column_headers[col_index]->up;
            node->up                  = last_node_in_column;
            last_node_in_column->down = node;
            // update column header bonds with current node (it is the last node
            // in the column now)
            column_headers[col_index]->up       = node;
            column_headers[col_index]->up->down = column_headers[col_index];
            // set column pointer
            node->column_header = column_headers[col_index];
            // set node count
            node->nodeCount = 0;
            // increment node count of column header
            node->column_header->nodeCount++;
            // update pervious row node
            pervious_row_node = node;
            // update first node in row
            if (!first_node_in_row) first_node_in_row = node;
        }
        // link last node in row with first node in row
        if (first_node_in_row) {
            node->right             = first_node_in_row;
            first_node_in_row->left = node;
        }
    }

    // free column headers array
    free(column_headers);

    return head;
}

// merge sort circular two-way horizontal linked list of nodes
// head is the first node in the list

// get middle of linked list
Node* getMiddle(Node* head) {
    if (!head) return head;
    Node* slow = head;
    Node* fast = head->right;
    while (fast) {
        fast = fast->right;
        if (fast) {
            slow = slow->right;
            fast = fast->right;
        }
    }
    return slow;
}

// merge two sorted linked lists with custom comparator
Node* merge(Node* first, Node* second, bool (*comparator)(Node*, Node*)) {
    if (!first) return second;
    if (!second) return first;

    // pick the smaller value
    if (comparator(first, second)) {
        first->right       = merge(first->right, second, comparator);
        first->right->left = first;
        first->left        = NULL;
        return first;
    } else {
        second->right       = merge(first, second->right, comparator);
        second->right->left = second;
        second->left        = NULL;
        return second;
    }
}

// merge sort linked list with custom comparator
Node* mergeSort(Node* head, bool (*comparator)(Node*, Node*)) {
    if (!head || !head->right) return head;

    // split list in two halves
    Node* middle  = getMiddle(head);
    Node* second  = middle->right;
    middle->right = NULL;

    // sort halves
    Node* left  = mergeSort(head, comparator);
    Node* right = mergeSort(second, comparator);

    // merge halves
    return merge(left, right, comparator);
}

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
    if (DEBUG) printf("In function createMonkeyFistMesh2()\n");

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
            // if (DEBUG)
            // printf("Freeing node r%d, c%d\n", node->row_ID, node->column_ID);
            free(node);
            node = next_node;
        }
        Node* next_column_header = column_header->right;
        // if (DEBUG) printf("Freeing column header c%d\n",
        // column_header->column_ID);
        free(column_header);
        column_header = next_column_header;
    }
    // if (DEBUG) printf("Freeing head\n");
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
    // if (DEBUG) printf("In solutionToHexadoku\n");
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