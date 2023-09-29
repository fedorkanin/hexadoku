#include "Hexadoku.h"

// read 16x16 hexadoku from stdin. Unset positions are represented by 0.
uint8_t** readIn() {
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
                DEBUG_PRINTF("Invalid row %d.\n", i + 1);
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
                DEBUG_PRINTF("Invalid column %d.\n", i + 1);
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
                DEBUG_PRINTF("Invalid block %d.\n", i + 1);
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