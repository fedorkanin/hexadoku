#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUDOKU_SIZE 16
#define MAX_ROW 100
#define MAX_COLUMN 100
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

void PushToBoolVector(BoolVector* v, bool value) {
  if (v->size == v->capacity) {
    v->capacity = v->capacity == 0 ? 1 : 2 * v->capacity;
    v->data = (bool*)realloc(v->data, v->capacity * sizeof(bool));
  }
  v->data[v->size++] = value;
}

void popFromBoolVector(BoolVector* v) {
  if (v->size > 0) v->size--;
}

void FreeBoolVector(BoolVector* v) {
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

void insertToBoolVector2D(BoolVector2D* v, size_t index, BoolVector* value) {
  if (v->size == v->capacity) {
    v->capacity = v->capacity == 0 ? 1 : 2 * v->capacity;
    v->data = (BoolVector**)realloc(v->data, v->capacity * sizeof(BoolVector*));
  }
  for (size_t i = v->size; i > index; i--) {
    v->data[i] = v->data[i - 1];
  }
  v->data[index] = value;
  v->size++;
}

void printBoolVector2D(BoolVector2D* v) {
  for (size_t i = 0; i < v->size; i++) {
    for (size_t j = 0; j < v->data[i]->size; j++) {
      printf("%d ", v->data[i]->data[j]);
    }
    printf("\n");
  }
}

void freeBoolVector2D(BoolVector2D* v) {
  for (size_t i = 0; i < v->size; i++) FreeBoolVector(v->data[i]);
  free(v->data);
  free(v);
}

BoolVector2D* hexadokuToExactCover(uint8_t** hexadoku) {
  if (DEBUG) printf("Converting hexadoku to exact cover...\n");
  // possible candidates for each cell
  size_t rows_number = pow(SUDOKU_SIZE, 3);
  // 4 constraints for each cell
  size_t columns_number = pow(SUDOKU_SIZE, 2) * 4;
  // initialize exact cover matrix
  BoolVector2D* exact_cover = createBoolVector2D(rows_number);

  // push rows
  for (size_t i = 0; i < rows_number; i++) {
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
  struct node* column;

  int rowID;
  int columnID;
  int nodeCount;
} Node;

Node* initNode(int rowID, int columnID) {
  Node* node = (Node*)malloc(sizeof(Node));
  node->left = node;
  node->right = node;
  node->up = node;
  node->down = node;
  node->column = NULL;
  node->rowID = rowID;
  node->columnID = columnID;
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

void pushToNodeVector(NodeVector* v, Node* node) {
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

// functions to get circular index in any direction
int getRight(int index, int size) { return (index + 1) % size; }
int getLeft(int index, int size) { return (index - 1 + size) % size; }
int getUp(int index, int size) { return (index - 1 + size) % size; }
int getDown(int index, int size) { return (index + 1) % size; }

// function to get first non-null neighbor index in a matrix
// takes in 'R' for right, 'L' for left, 'U' for up, 'D' for down
int getFirstNeighbor(BoolVector2D* bool_matrix, size_t i, size_t j,
                     char direction) {
  // use "case"
  size_t size = bool_matrix->data[0]->capacity;
  switch (direction) {
    case 'R':
      for (size_t k = getRight(j, size); k != j; k = getRight(k, size)) {
        if (bool_matrix->data[i]->data[k]) {
          return k;
        }
      }
      break;
    case 'L':
      for (size_t k = getLeft(j, size); k != j; k = getLeft(k, size)) {
        if (bool_matrix->data[i]->data[k]) {
          return k;
        }
      }
      break;
    case 'U':
      for (size_t k = getUp(i, size); k != i; k = getUp(k, size)) {
        if (bool_matrix->data[k]->data[j]) {
          return k;
        }
      }
      break;
    case 'D':
      for (size_t k = getDown(i, size); k != i; k = getDown(k, size)) {
        if (bool_matrix->data[k]->data[j]) {
          return k;
        }
      }
      break;
    default:
      break;
  }
  // if no neighbor found, return the same index
  if (direction == 'R' || direction == 'L') {
    return j;
  } else {
    return i;
  }
}

void setNeighbors(BoolVector2D* exact_cover, Node*** node_matrix, int i,
                  int j) {
  // set left and right neighbors
  int left = getFirstNeighbor(exact_cover, i, j, 'L');
  int right = getFirstNeighbor(exact_cover, i, j, 'R');
  node_matrix[i][j]->left = node_matrix[i][left];
  node_matrix[i][j]->right = node_matrix[i][right];
  // set up and down neighbors
  int up = getFirstNeighbor(exact_cover, i, j, 'U');
  int down = getFirstNeighbor(exact_cover, i, j, 'D');
  node_matrix[i][j]->up = node_matrix[up][j];
  node_matrix[i][j]->down = node_matrix[down][j];
}

void setId(Node* node, int rowID, int columnID) {
  node->rowID = rowID;
  node->columnID = columnID;
}

// create 4 way linked mesh
// called Monkey Fist Matrix because it resembles a monkey fist knot
// https://en.wikipedia.org/wiki/Monkey's_fist
Node* createMonkeyFistMatrix(BoolVector2D* exact_cover, Node*** node_matrix) {
  // pointer to list header of first column
  Node* header = initNode(-1, -1);

  // first number is just ones
  size_t rows_number = exact_cover->capacity - 1;
  size_t columns_number = exact_cover->data[0]->capacity;

  // create matrix dynamically
  node_matrix = (Node***)malloc(sizeof(Node**) * (rows_number + 1));
  for (size_t i = 0; i <= rows_number; i++) {
    node_matrix[i] = (Node**)malloc(sizeof(Node*) * columns_number);
    for (size_t j = 0; j < columns_number; j++) {
      node_matrix[i][j] = initNode(i, j);
    }
  }

  // one extra row for column headers
  // cycle through each element of exact cover matrix
  for (size_t i = 0; i <= rows_number; i++) {
    for (size_t j = 0; j < columns_number; j++) {
      // if element is false, skip
      if (!exact_cover->data[i]->data[j]) {
        continue;
      }
      // element is true, create node
      // if first row, increase column header nodeCount
      if (i) node_matrix[0][j]->nodeCount++;
      // add pointer to column header for this column node
      // printf("i: %ld, j: %ld\n", i, j);
      node_matrix[i][j]->column = node_matrix[0][j];
      // set row and column id's
      setId(node_matrix[i][j], i, j);
      // link node to neighbors
      setNeighbors(exact_cover, node_matrix, i, j);
    }
  }

  // link header right to first column header
  header->right = node_matrix[0][0];
  // link header left to last column header
  header->left = node_matrix[0][columns_number - 1];

  // link matrix first column header left to header
  node_matrix[0][0]->left = header;
  // link matrix last column header right to header
  node_matrix[0][columns_number - 1]->right = header;

  return header;
}

// cover given node completely
void coverColumn(Node* target_node) {
  Node *row_node, *right_node;
  Node* column_node = target_node->column;

  // unlink column header (thank you, Donald Knuth)
  column_node->left->right = column_node->right;
  column_node->right->left = column_node->left;

  // move down the column and remove all nodes in the same row
  for (row_node = column_node->down; row_node != column_node;
       row_node = row_node->down) {
    // move right in the row and unlink all nodes
    for (right_node = row_node->right; right_node != row_node;
         right_node = right_node->right) {
      // thank you, Donald Knuth
      right_node->up->down = right_node->down;
      right_node->down->up = right_node->up;
      // decrement column header count
      // WARNING, ERROR POSSIBLE HERE
      right_node->column->nodeCount--;
    }
  }
}

// uncover given node completely
void uncoverColumn(Node* target_node) {
  Node *row_node, *left_node;
  Node* column_node = target_node->column;

  // move up the column and reinsert all nodes in the same row
  for (row_node = column_node->up; row_node != column_node;
       row_node = row_node->up) {
    // move left in the row and reinsert all nodes
    for (left_node = row_node->left; left_node != row_node;
         left_node = left_node->left) {
      // thank you, Donald Knuth
      left_node->up->down = left_node;
      left_node->down->up = left_node;
      // increment column header count
      // WARNING, ERROR POSSIBLE HERE
      left_node->column->nodeCount++;
    }
  }
  // relink column header (thank you, Donald Knuth)
  column_node->left->right = column_node;
  column_node->right->left = column_node;
}

// find column with least nodes
Node* findColumnWithLeastNodes(Node* header) {
  Node* column = header->right;
  Node* least_node_column = column;
  size_t least_node_count = column->nodeCount;
  // cycle through columns
  for (column = column->right; column != header; column = column->right) {
    // if column has less nodes than least_node_column, update
    if ((size_t)column->nodeCount < least_node_count) {
      least_node_column = column;
      least_node_count = column->nodeCount;
    }
  }
  return least_node_column;
}

void printSolutions(Node** solutions, size_t nSolution) {
  printf("Solutions:\n");
  for (size_t i = 0; i < nSolution; i++) {
    printf("Solution %zu:\n", i);
    for (size_t j = 0; j < MAX_ROW; j++) {
      if (solutions[i][j].rowID == -1) break;
      printf("Row: %d, Column: %d\n", solutions[i][j].rowID,
             solutions[i][j].columnID);
    }
  }
}

// search for exact cover solutions
void search(size_t k, NodeVector* solution, Node* header) {
  Node* row_node;
  Node* right_node;
  Node* left_node;
  Node* column_node;

  // if there are no more columns, we have a solution
  if (header->right == header) {
    // add solution to solutions
    printf("Solution %zu:\n", solution->size);
    printSolutions(solution->data, solution->size);
    return;
  }

  // choose column with least nodes
  column_node = findColumnWithLeastNodes(header);
  // cover chosen column
  coverColumn(column_node);

  for (row_node = column_node->down; row_node != column_node;
       row_node = row_node->down) {
    // add row node to solution
    pushToNodeVector(solution, row_node);
    // cover all columns in the same row
    for (right_node = row_node->right; right_node != row_node;
         right_node = right_node->right)
      coverColumn(right_node);
    // search for solutions
    search(k + 1, solution, header);
    // remove row node from solution
    popFromNodeVector(solution);
    // uncover all columns in the same row
    column_node = row_node->column;
    for (left_node = row_node->left; left_node != row_node;
         left_node = left_node->left)
      uncoverColumn(left_node);
  }
  // uncover chosen column
  uncoverColumn(column_node);
}

int main(void) {
  uint8_t** hexadoku = readIn();
  if (hexadoku == NULL) {
    printf("Invalid input.\n");
    return 1;
  }
  printf("Got hexadoku:\n");
  printHexadoku(hexadoku);
  BoolVector2D* exact_cover = hexadokuToExactCover(hexadoku);

  // free memory
  for (size_t i = 0; i < MAX_ROW; i++) free(hexadoku[i]);
  free(hexadoku);
  return 0;
}
