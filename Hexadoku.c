#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUDOKU_SIZE 16
#define DEBUG 1

size_t LINE_WIDTH = 4 * SUDOKU_SIZE + 1;
size_t LINE_HEIGHT = 2 * SUDOKU_SIZE + 1;

bool isDelimiterStringValid(char* string, bool is_dashed) {
  size_t line_width = (size_t)SUDOKU_SIZE * 4 + 1;
  if (strlen(string) != line_width) {
    if (DEBUG) printf("Invalid line width.\n");
    if (DEBUG) printf("Expected: %zu, got: %zu\n", line_width, strlen(string));
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

// convert string to array of uint8_t
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
        array[(i - 2) / 4] = (string[i] == ' ') ? 0 : string[i] - 'a' + 1;
      else {
        if (DEBUG) printf("Invalid letter at position %zu.\n", i);
        free(array);
        return NULL;
      }
    } else if (string[i] == ' ') {
      continue;
    } else {
      if (DEBUG) printf("Invalid character at position %zu.\n", i);
      free(array);
      return NULL;
    }
  }

  return array;
}

// read string from stdin using getline, returns '\0' string in case
// of failure
char* getString() {
  char* string = NULL;
  size_t size = 0;
  if (getline(&string, &size, stdin) == -1) {
    free(string);
    string = (char*)malloc(sizeof(char));
    string[0] = '\0';
  }
  // remove trailing newline
  size_t len = strlen(string);
  if (len > 0 && string[len - 1] == '\n') string[len - 1] = '\0';
  return string;
}

// read 16x16 hexadoku from stdin. Unset positions are represented by 0.
uint8_t** readIn() {
  uint8_t** hexadoku = (uint8_t**)malloc(16 * sizeof(char*));
  // read first line
  char* line = getString();
  if (!isDelimiterStringValid(line, false)) {
    if (DEBUG) printf("Invalid first line.\n");
    free(line);
    free(hexadoku);
    return NULL;
  }

  // read lines and delimiter lines
  for (size_t i = 0; i < LINE_HEIGHT - 2; i++) {
    free(line);
    if (i % 2 == 0) {
      // read line with letters
      line = getString();
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
      line = getString();
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
    for (size_t j = 0; j < 16; j++) free(hexadoku[j]);
    free(hexadoku);
    return NULL;
  }
  free(line);

  return hexadoku;
}

// print 16x16 hexadoku
void printHexadoku(uint8_t** hexadoku) {
  // print first line
  for (size_t i = 0; i < LINE_WIDTH - 1; i++) {
    if (i % 4 == 0) {
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

typedef struct {
  int* data;
  size_t size;
  size_t capacity;
} IntVector;

IntVector* createIntVector(size_t capacity) {
  IntVector* vector = (IntVector*)malloc(sizeof(IntVector));
  vector->data = (int*)malloc(capacity * sizeof(int));
  vector->size = 0;
  vector->capacity = capacity;
  return vector;
}

void pushToIntVector(IntVector* v, int value) {
  if (v->size == v->capacity) {
    v->capacity = v->capacity == 0 ? 1 : 2 * v->capacity;
    v->data = (int*)realloc(v->data, v->capacity * sizeof(int));
  }
  v->data[v->size++] = value;
}

void popFromIntVector(IntVector* v) {
  if (v->size > 0) {
    v->size--;
  }
}

void printIntVector(IntVector* v) {
  for (size_t i = 0; i < v->size; i++) {
    printf("%d ", v->data[i]);
  }
  printf("\n");
}

void freeIntVector(IntVector* v) {
  free(v->data);
  free(v);
}

typedef struct {
  bool* data;
  size_t size;
  size_t capacity;
  bool is_active;
} BoolVector;

BoolVector* createBoolVector(size_t capacity) {
  BoolVector* vector = (BoolVector*)malloc(sizeof(BoolVector));
  vector->data = (bool*)malloc(capacity * sizeof(bool));
  vector->size = 0;
  vector->capacity = capacity;
  vector->is_active = true;
  return vector;
}

void pushToBoolVector(BoolVector* v, bool value) {
  if (v->size == v->capacity) {
    v->capacity = v->capacity == 0 ? 1 : 2 * v->capacity;
    v->data = (bool*)realloc(v->data, v->capacity * sizeof(bool));
  }
  v->data[v->size++] = value;
}

void popFromBoolVector(BoolVector* v) {
  if (v->size > 0) v->size--;
}

void freeBoolVector(BoolVector* v) {
  free(v->data);
  free(v);
}

typedef struct {
  BoolVector** data;
  size_t size;
  size_t capacity;
} BoolVector2D;

BoolVector2D* createBoolVector2D(size_t capacity) {
  BoolVector2D* vector = (BoolVector2D*)malloc(sizeof(BoolVector2D));
  vector->data = (BoolVector**)malloc(capacity * sizeof(BoolVector*));
  vector->size = 0;
  vector->capacity = capacity;
  return vector;
}

void pushToBoolVector2D(BoolVector2D* v, BoolVector* value) {
  if (v->size == v->capacity) {
    v->capacity = v->capacity == 0 ? 1 : 2 * v->capacity;
    v->data = (BoolVector**)realloc(v->data, v->capacity * sizeof(BoolVector*));
  }
  v->data[v->size++] = value;
}

void printBoolVector2D(BoolVector2D* v) {
  for (size_t i = 0; i < v->size; i++) {
    for (size_t j = 0; j < v->data[i]->size; j++) {
      printf("%d ", v->data[i]->data[j]);
    }
    printf("\n");
  }
}

BoolVector2D* boolArrayToVector2D(bool array[][4], size_t height,
                                  size_t width) {
  BoolVector2D* vector = createBoolVector2D(height);
  for (size_t i = 0; i < height; i++) {
    BoolVector* row = createBoolVector(width);
    for (size_t j = 0; j < width; j++) {
      pushToBoolVector(row, array[i][j]);
    }
    pushToBoolVector2D(vector, row);
  }
  return vector;
}

void freeBoolVector2D(BoolVector2D* v) {
  for (size_t i = 0; i < v->size; i++) freeBoolVector(v->data[i]);
  free(v->data);
  free(v);
}

BoolVector2D* hexadokuToExactCover(uint8_t** hexadoku) {
  if (DEBUG) printf("Converting hexadoku to exact cover...\n");
  // possible candidates for each cell
  size_t rows_number = pow(SUDOKU_SIZE, 3) + 1;
  // 4 constraints for each cell
  size_t columns_number = pow(SUDOKU_SIZE, 2) * 4;
  // initialize exact cover matrix
  BoolVector2D* exact_cover = createBoolVector2D(rows_number);

  // push first row consisting of 1s (required for transformaing to monkey fist)
  pushToBoolVector2D(exact_cover, createBoolVector(columns_number));
  exact_cover->data[0]->size = exact_cover->data[0]->capacity;
  for (size_t i = 0; i < columns_number; i++)
    exact_cover->data[0]->data[i] = true;

  // Push rows
  for (size_t i = 1; i < rows_number; i++) {
    pushToBoolVector2D(exact_cover, createBoolVector(columns_number));
    exact_cover->data[i]->size = exact_cover->data[i]->capacity;
  }
  // fill with zeros
  for (size_t i = 1; i < rows_number; i++)
    for (size_t j = 0; j < columns_number; j++)
      exact_cover->data[i]->data[j] = false;

  // print width and height of BoolVector2D
  if (DEBUG)
    printf("Matrix width: %ld, height: %ld\n", exact_cover->data[0]->capacity,
           exact_cover->capacity);

  // fill exact cover matrix
  for (size_t i = 0; i < SUDOKU_SIZE; i++) {
    for (size_t j = 0; j < SUDOKU_SIZE; j++) {
      // row constraint satisfied
      for (size_t k = 0; k < SUDOKU_SIZE; k++) {
        size_t row = i * SUDOKU_SIZE * SUDOKU_SIZE + j * SUDOKU_SIZE + k;
        size_t column = i * SUDOKU_SIZE + j;
        exact_cover->data[row]->data[column] = true;
      }
      // column constraint satisfied
      for (size_t k = 0; k < SUDOKU_SIZE; k++) {
        size_t row = i * SUDOKU_SIZE * SUDOKU_SIZE + j * SUDOKU_SIZE + k;
        size_t column = SUDOKU_SIZE * SUDOKU_SIZE + i * SUDOKU_SIZE + k;
        exact_cover->data[row]->data[column] = true;
      }
      // block constraint satisfied
      for (size_t k = 0; k < SUDOKU_SIZE; k++) {
        size_t row = i * SUDOKU_SIZE * SUDOKU_SIZE + j * SUDOKU_SIZE + k;
        size_t column = 2 * SUDOKU_SIZE * SUDOKU_SIZE +
                        (i / 4 * 4 + j / 4) * SUDOKU_SIZE + k;
        exact_cover->data[row]->data[column] = true;
      }
      // cell constraint satisfied
      if (hexadoku[i][j] != 0) {
        size_t row = i * SUDOKU_SIZE * SUDOKU_SIZE + j * SUDOKU_SIZE +
                     hexadoku[i][j] - 1;
        size_t column = 3 * SUDOKU_SIZE * SUDOKU_SIZE + i * SUDOKU_SIZE + j;
        exact_cover->data[row]->data[column] = true;
      }
    }
  }

  return exact_cover;
}

// ==================== EXACT COVER SOLVER ====================
// exact cover solver based on DLX algorithm by Donald Knuth
// https://arxiv.org/pdf/cs/0011047.pdf

// struct node
typedef struct node {
  struct node* left;
  struct node* right;
  struct node* up;
  struct node* down;
  struct node* column_header;

  int row_ID;  // if -1, then it's a column header
  int column_ID;
  int nodeCount;
} Node;

Node* initNode(int rowID, int columnID) {
  Node* node = (Node*)malloc(sizeof(Node));
  node->left = node;
  node->right = node;
  node->up = node;
  node->down = node;
  node->column_header = NULL;
  node->row_ID = rowID;
  node->column_ID = columnID;
  node->nodeCount = 0;
  return node;
}

// struct node vector
typedef struct {
  Node** data;
  size_t size;
  size_t capacity;
} NodeVector;

NodeVector* createNodeVector(size_t capacity) {
  NodeVector* v = (NodeVector*)malloc(sizeof(NodeVector));
  v->data = (Node**)malloc(capacity * sizeof(Node*));
  v->size = 0;
  v->capacity = capacity;
  return v;
}

void PushToNodeVector(NodeVector* v, Node* node) {
  if (v->size == v->capacity) {
    v->capacity *= 2;
    v->data = (Node**)realloc(v->data, v->capacity * sizeof(Node*));
  }
  v->data[v->size++] = node;
}

void popFromNodeVector(NodeVector* v) {
  free(v->data[v->size - 1]);
  if (v->size > 0) {
    v->size--;
  }
}

void freeNodeVector(NodeVector* v) {
  free(v->data);
  free(v);
}

// index may be negative or greater than size
int getCircularIndex(int index, int size) {
  if (index < 0) {
    return size + index;
  } else if (index >= size) {
    return index - size;
  } else {
    return index;
  }
}

// funciton returns horizontal index of the closest left neighbor with value 1
size_t leftNeighborId(BoolVector2D* exact_cover, size_t row, size_t column) {
  size_t left_neighbor_id = column;
  do {
    left_neighbor_id =
        getCircularIndex(left_neighbor_id - 1, exact_cover->data[0]->capacity);
  } while (exact_cover->data[row]->data[left_neighbor_id] == false);
  return left_neighbor_id;
}
// funciton returns horizontal index of the closest right neighbor with value 1
size_t rightNeighborId(BoolVector2D* exact_cover, size_t row, size_t column) {
  size_t right_neighbor_id = column;
  do {
    right_neighbor_id =
        getCircularIndex(right_neighbor_id + 1, exact_cover->data[0]->capacity);
  } while (exact_cover->data[row]->data[right_neighbor_id] == false);
  return right_neighbor_id;
}
// funciton returns vertical index of the closest top neighbor with value 1
size_t topNeighborId(BoolVector2D* exact_cover, size_t row, size_t column) {
  size_t top_neighbor_id = row;
  do {
    top_neighbor_id =
        getCircularIndex(top_neighbor_id - 1, exact_cover->capacity);
  } while (exact_cover->data[top_neighbor_id]->data[column] == false);
  return top_neighbor_id;
}
// funciton returns vertical index of the closest bottom neighbor with value 1
size_t bottomNeighborId(BoolVector2D* exact_cover, size_t row, size_t column) {
  size_t bottom_neighbor_id = row;
  do {
    bottom_neighbor_id =
        getCircularIndex(bottom_neighbor_id + 1, exact_cover->capacity);
  } while (exact_cover->data[bottom_neighbor_id]->data[column] == false);
  return bottom_neighbor_id;
}

// create monkey fist node mesh form exact cover matrix represented by
// BoolVector2D. Exact cover matrix must have an additiona first row of 1's
// returns pointer to head node
Node* createMonkeyFistMesh(BoolVector2D* exact_cover) {
  // create head node
  Node* head = initNode(-1, -1);

  size_t mesh_width = exact_cover->data[0]->capacity;
  size_t mesh_height = exact_cover->capacity;

  // create temporary 2d matrix of nodes
  Node*** node_matrix = (Node***)malloc(mesh_height * sizeof(Node**));
  for (size_t i = 0; i < mesh_height; i++)
    node_matrix[i] = (Node**)malloc(mesh_width * sizeof(Node*));
  // initialize node matrix with NULL
  for (size_t i = 0; i < mesh_height; i++)
    for (size_t j = 0; j < mesh_width; j++) node_matrix[i][j] = NULL;

  // create column nodes
  for (size_t i = 0; i < mesh_width; i++) {
    Node* column_node = initNode(-1, i);
    // link with left neighbor, link left neighbor with this node
    column_node->left = i ? node_matrix[0][i - 1] : head;
    column_node->left->right = column_node;
    // set column pointer
    column_node->column_header = column_node;
    // set node count
    column_node->nodeCount = 0;
    // add to node matrix
    node_matrix[0][i] = column_node;
  }
  // link last column node with head node, first one is already linked
  node_matrix[0][mesh_width - 1]->right = head;
  head->left = node_matrix[0][mesh_width - 1];

  // create mesh nodes
  for (size_t row_id = 1; row_id < mesh_height; row_id++) {
    Node* node;
    for (size_t column_id = 0; column_id < mesh_width; column_id++) {
      if (exact_cover->data[row_id]->data[column_id] == false) continue;
      // create node
      node = initNode(row_id - 1, column_id);
      // add to node matrix
      node_matrix[row_id][column_id] = node;
      // link node with horizontral neighbors
      node->left =
          node_matrix[row_id][leftNeighborId(exact_cover, row_id, column_id)];
      node->right =
          node_matrix[row_id][rightNeighborId(exact_cover, row_id, column_id)];
      // link horizontal neighbors with this node
      if (node->left) node->left->right = node;
      if (node->right) node->right->left = node;
      // link node with vertical neighbors
      node->up =
          node_matrix[topNeighborId(exact_cover, row_id, column_id)][column_id];
      node->down = node_matrix[bottomNeighborId(exact_cover, row_id, column_id)]
                              [column_id];
      // link vertical neighbors with this node
      if (node->up) node->up->down = node;
      if (node->down) node->down->up = node;
      // set column pointer
      node->column_header = node_matrix[0][column_id];
      // set node count
      node->nodeCount = 0;
      // increment node count of column header
      node->column_header->nodeCount++;
    }
  }

  // free node matrix
  for (size_t i = 0; i < mesh_height; i++) free(node_matrix[i]);
  free(node_matrix);

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
             node->right->column_ID, node->up->row_ID, node->down->row_ID);
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
        printf("Node r%d, c%d has NULL column header pointer \n", node->row_ID,
               node->column_ID);
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
      if (DEBUG)
        printf("Freeing node r%d, c%d\n", node->row_ID, node->column_ID);
      free(node);
      node = next_node;
    }
    Node* next_column_header = column_header->right;
    if (DEBUG) printf("Freeing column header c%d\n", column_header->column_ID);
    free(column_header);
    column_header = next_column_header;
  }
  if (DEBUG) printf("Freeing head\n");
  free(head);
}

// cover column
void cover(Node* column_header) {
  // unlink column header from horizontal neighbors
  column_header->left->right = column_header->right;
  column_header->right->left = column_header->left;
  // iterate over column
  for (Node* node = column_header->down; node != column_header;
       node = node->down) {
    // iterate over row
    for (Node* row_node = node->right; row_node != node;
         row_node = row_node->right) {
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
  for (Node* node = column_header->up; node != column_header; node = node->up) {
    // iterate over row
    for (Node* row_node = node->left; row_node != node;
         row_node = row_node->left) {
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
  Node* min_column = head->right;
  Node* column_header = head->right->right;
  while (column_header != head) {
    if (column_header->nodeCount < min_column->nodeCount) {
      min_column = column_header;
    }
    column_header = column_header->right;
  }
  return min_column;
}

// print all solutions of monkey fist mesh
void searchSolutions(Node* head, IntVector* solution) {
  if (DEBUG) printf("In searchSolutions\n");
  Node* row_node;
  Node* right_node;
  Node* left_node;
  Node* column;

  // if there are no more columns, we found a solution
  if (head->right == head) {
    // print solution
    printf("Solution: ");
    printIntVector(solution);
    return;
  }

  // cover column with minimum node count
  column = getMinColumn(head);
  if (DEBUG)
    printf("Covering column %d, node count %d\n", column->column_ID,
           column->nodeCount);
  cover(column);

  // iterate over column
  for (row_node = column->down; row_node != column; row_node = row_node->down) {
    if (DEBUG) printf("Iterating over row %d\n", row_node->row_ID);
    // add row to solution
    pushToIntVector(solution, row_node->row_ID);

    for (right_node = row_node->right; right_node != row_node;
         right_node = right_node->right)
      cover(right_node->column_header);

    // search for solutions
    searchSolutions(head, solution);

    // if solution is not possible, backtrack and uncover column
    popFromIntVector(solution);

    column = row_node->column_header;
    for (left_node = row_node->left; left_node != row_node;
         left_node = left_node->left)
      uncover(left_node->column_header);
  }
  uncover(column);
}

int main(void) {
  uint8_t** hexadoku = readIn();
  if (hexadoku == NULL) {
    printf("Invalid input.\n");
    return 1;
  }
  printf("Got hexadoku:\n");
  printHexadoku(hexadoku);
  // BoolVector2D* exact_cover = hexadokuToExactCover(hexadoku);

  // create matrix to test
  bool test_matrix[8][4] = {{1, 1, 1, 1}, {1, 0, 1, 0}, {0, 1, 0, 1},
                            {0, 1, 1, 0}, {1, 0, 0, 0}, {0, 1, 0, 0},
                            {0, 0, 1, 0}, {0, 0, 0, 1}};
  BoolVector2D* exact_cover = boolArrayToVector2D(test_matrix, 8, 4);
  if (DEBUG) printf("Got exact cover:\n");
  printBoolVector2D(exact_cover);

  // create monkey fist mesh and print it
  Node* head = createMonkeyFistMesh(exact_cover);
  validateMonkeyFistMesh(head);
  printf("Monkey fist mesh:\n");
  printMonkeyFistMesh(head);

  // search for solutions
  IntVector* solution = createIntVector(0);
  searchSolutions(head, solution);

  // free memory
  // free hexadoku
  for (size_t i = 0; i < SUDOKU_SIZE; i++) free(hexadoku[i]);
  free(hexadoku);
  // free exact cover bool vector matrix
  freeBoolVector2D(exact_cover);
  // free monkey fist mesh
  freeMonkeyFistMesh(head);
  // free solution vector
  freeIntVector(solution);
  return 0;
}
