#include "Hexadoku.h"

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