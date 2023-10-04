#include "Hexadoku.h"

bool isHexadokuValid(Sudoku* hex) {
    // Check rows
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        bool row[SUDOKU_SIZE] = {0};
        for (int j = 0; j < SUDOKU_SIZE; j++) {
            if (hex->cell_matrix[i][j] == 0) continue;
            if (row[hex->cell_matrix[i][j] - 1] != false) {
                DEBUG_PRINTF("Invalid row %d.\n", i + 1);
                return false;
            }
            row[hex->cell_matrix[i][j] - 1] = true;
        }
    }

    // Check columns
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        bool column[SUDOKU_SIZE] = {0};
        for (int j = 0; j < SUDOKU_SIZE; j++) {
            if (hex->cell_matrix[j][i] == 0) continue;
            if (column[hex->cell_matrix[j][i] - 1] != false) {
                DEBUG_PRINTF("Invalid column %d.\n", i + 1);
                return false;
            }
            column[hex->cell_matrix[j][i] - 1] = true;
        }
    }

    // Check blocks
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        bool block[SUDOKU_SIZE] = {0};
        for (int j = 0; j < SUDOKU_SIZE; j++) {
            int row    = i / BOX_SIZE * BOX_SIZE + j / BOX_SIZE;
            int column = i % BOX_SIZE * BOX_SIZE + j % BOX_SIZE;
            if (hex->cell_matrix[row][column] == 0) continue;
            if (block[hex->cell_matrix[row][column] - 1] != false) {
                DEBUG_PRINTF("Invalid block %d.\n", i + 1);
                return false;
            }
            block[hex->cell_matrix[row][column] - 1] = true;
        }
    }

    return true;
}

void printHexadoku(Sudoku* hex) {
    // Print first line
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
            // Print line with letters
            for (size_t j = 0; j < LINE_WIDTH - 1; j++) {
                if (j % 16 == 0) {
                    printf("|");
                } else if ((j - 2) % 4 == 0) {
                    // Letter position
                    char letter = hex->cell_matrix[i / 2][(j - 2) / 4];
                    printf("%c", letter == 0 ? ' ' : letter + 'a' - 1);
                } else {
                    printf(" ");
                }
            }
            printf("|\n");
        } else {
            // Print delimiter line
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

void freeHexadoku(Sudoku* hex) {
    if (hex == NULL) {
        return;
    }

    // Free the hexadoku grid
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        free(hex->cell_matrix[i]);
    }
    free(hex->cell_matrix);

    // Free the solution vector
    freeIntVector(hex->solution);

    // Free the Sudoku structure itself
    free(hex);
}
