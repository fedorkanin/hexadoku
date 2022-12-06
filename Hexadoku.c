#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
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

char* strToCharArray(char* string) {
  if (strlen(string) != LINE_WIDTH) {
    if (DEBUG) printf("Invalid line width.\n");
    return NULL;
  }

  char* array = (char*)malloc(SUDOKU_SIZE * sizeof(char));
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

// read 16x16 hexadoku from stdin
char** readIn() {
  char** hexadoku = (char**)malloc(16 * sizeof(char*));
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
      hexadoku[i / 2] = strToCharArray(line);
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
void printHexadoku(char** hexadoku) {
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
  char* data;
  size_t size;
  size_t capacity;
} CharVector;

CharVector* InitCharVector(size_t capacity) {
  CharVector* vector = (CharVector*)malloc(sizeof(CharVector));
  vector->data = (char*)malloc(capacity * sizeof(char));
  vector->size = 0;
  vector->capacity = capacity;
  return vector;
}

void PushToCharVector(CharVector* v, char value) {
  if (v->size == v->capacity) {
    v->capacity = v->capacity == 0 ? 1 : 2 * v->capacity;
    v->data = (char*)realloc(v->data, v->capacity * sizeof(char));
  }
  v->data[v->size++] = value;
}

void FreeCharVector(CharVector* v) {
  free(v->data);
  free(v);
}

typedef struct {
  CharVector** data;
  size_t size;
  size_t capacity;
} CharVector2D;

CharVector2D* InitCharVector2D(size_t capacity) {
  CharVector2D* vector = (CharVector2D*)malloc(sizeof(CharVector2D));
  vector->data = (CharVector**)malloc(capacity * sizeof(CharVector*));
  vector->size = 0;
  vector->capacity = capacity;
  return vector;
}

void PushToCharVector2D(CharVector2D* v, CharVector* value) {
  if (v->size == v->capacity) {
    v->capacity = v->capacity == 0 ? 1 : 2 * v->capacity;
    v->data = (CharVector**)realloc(v->data, v->capacity * sizeof(CharVector*));
  }
  v->data[v->size++] = value;
}

char* get2D(CharVector2D* v, size_t i, size_t j) {
  return &v->data[i]->data[j];
}

void FreeCharVector2D(CharVector2D* v) {
  for (size_t i = 0; i < v->size; i++) FreeCharVector(v->data[i]);
  free(v->data);
  free(v);
}

int main(void) {
  char** hexadoku = readIn();
  if (hexadoku == NULL) {
    printf("Invalid input.\n");
    return 1;
  }
  printf("Got hexadoku:\n");
  printHexadoku(hexadoku);

  // free hexadoku
  for (size_t i = 0; i < 16; i++) free(hexadoku[i]);
  free(hexadoku);
  return 0;
}
