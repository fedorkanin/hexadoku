#include "InputFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool isDelimiterStringValid(char* string, bool is_dashed) {
    size_t line_width = (int)SUDOKU_SIZE * 4 + 1;
    if (strlen(string) != line_width) {
        DEBUG_PRINTF("Invalid line width.\n");
        DEBUG_PRINTF("Expected: %zu, got: %zu\n", line_width, strlen(string));
        return false;
    }

    if (is_dashed) {
        for (size_t i = 0; i < line_width; i++) {
            if (i % 4 == 0 && string[i] != '+') {
                DEBUG_PRINTF("Invalid character at position %zu.\n", i);
                DEBUG_PRINTF("Expected: '+', got: '%c'\n", string[i]);
                return false;
            } else if (i % 4 && string[i] != ' ') {
                DEBUG_PRINTF("Invalid character at position %zu.\n", i);
                DEBUG_PRINTF("Expected: ' ', got: '%c'\n", string[i]);
                return false;
            }
        }
    } else {
        for (size_t i = 0; i < line_width; i++) {
            if (i % 4 == 0 && string[i] != '+') {
                DEBUG_PRINTF("Invalid character at position %zu.\n", i);
                DEBUG_PRINTF("Expected: '+', got: '%c'\n", string[i]);
                return false;
            } else if (i % 4 && string[i] != '-') {
                DEBUG_PRINTF("Invalid character at position %zu.\n", i);
                DEBUG_PRINTF("Expected: '-', got: '%c'\n", string[i]);
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
        DEBUG_PRINTF("Invalid line width.\n");
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
                DEBUG_PRINTF("Invalid letter at position %zu.\n", i);
                free(array);
                return NULL;
            }
        } else if (string[i] == ' ' && i % 16 != 0) {
            continue;
        } else {
            DEBUG_PRINTF("Invalid character at position %zu.\n", i);
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