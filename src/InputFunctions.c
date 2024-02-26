#include "InputFunctions.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t** readProgtest(void) {
    uint8_t** hexadoku = (uint8_t**)malloc(16 * sizeof(char*));

    // read first line
    char*     line = getString();
    if (!isDelimiterStringValid(line, false)) {
        DEBUG_PRINTF("Invalid first line.\n");
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
                DEBUG_PRINTF("Invalid line %zu.\n", i / 2 + 1);
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
                DEBUG_PRINTF("Invalid delimiter line %zu.\n", i / 2 + 1);
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
        DEBUG_PRINTF("Invalid last line.\n");
        free(line);
        for (int j = 0; j < 16; j++) free(hexadoku[j]);
        free(hexadoku);
        return NULL;
    }
    free(line);

    // check no characters are left in stdin
    line = getString();
    if (strlen(line) != 0) {
        DEBUG_PRINTF("Input after hexadoku.\n");
        free(line);
        for (int j = 0; j < 16; j++) free(hexadoku[j]);
        free(hexadoku);
        return NULL;
    }
    free(line);

    return hexadoku;
}

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
    if ((a >= 'a') && (a <= 'a' + SUDOKU_SIZE - 1)) return true;
    if (a == ' ') return true;
    return false;
}

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

void stripString(char* string) {
    int len = strlen(string);
    while (len > 0 && isspace(string[len - 1])) {
        string[len - 1] = '\0';
        len--;
    }
}

char* getString(void) {
    char*  string = NULL;
    size_t size   = 0;
    if (getline(&string, &size, stdin) == -1) {
        free(string);
        string    = (char*)malloc(sizeof(char));
        string[0] = '\0';
    }

    int len = strlen(string);
    if (len > 0 && string[len - 1] == '\n') string[len - 1] = '\0';
    stripString(string);

    return string;
}
