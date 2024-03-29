#include "ExactCover.h"

int indexInExactCoverMatrix(int row, int column, int num) {
    // + 1 because first row is header
    return row * SUDOKU_SIZE * SUDOKU_SIZE + column * SUDOKU_SIZE + num + 1;
}

int numFromExactCoverIndex(int index) { return index % SUDOKU_SIZE + 1; }

int columnFromExactCoverIndex(int index) {
    return (index / SUDOKU_SIZE) % SUDOKU_SIZE;
}

int rowFromExactCoverIndex(int index) {
    return index / (SUDOKU_SIZE * SUDOKU_SIZE);
}

// Iterate through cells, update header for each cell (row and column).
int createCellConstraints(BoolVector2D* exact_cover, int header) {
    for (int row = 0; row < SUDOKU_SIZE; row++) {
        for (int column = 0; column < SUDOKU_SIZE; column++, header++) {
            for (int num = 0; num < SUDOKU_SIZE; num++) {
                exact_cover->data[indexInExactCoverMatrix(row, column, num)]
                    ->data[header] = true;
            }
        }
    }
    return header;
}

// Iterate through rows, update header for a unique number in each row.
int createRowConstraints(BoolVector2D* exact_cover, int header) {
    for (int row = 0; row < SUDOKU_SIZE; row++)
        for (int num = 0; num < SUDOKU_SIZE; num++, header++)
            for (int column = 0; column < SUDOKU_SIZE; column++) {
                exact_cover->data[indexInExactCoverMatrix(row, column, num)]
                    ->data[header] = true;
            }
    return header;
}

// Iterate through columns, update header for a unique number in each column.
int createColumnConstraints(BoolVector2D* exact_cover, int header) {
    for (int column = 0; column < SUDOKU_SIZE; column++)
        for (int num = 0; num < SUDOKU_SIZE; num++, header++)
            for (int row = 0; row < SUDOKU_SIZE; row++) {
                exact_cover->data[indexInExactCoverMatrix(row, column, num)]
                    ->data[header] = true;
            }
    return header;
}

// Iterate through top left corner of each box, update header for a unique
// number in each box.
int createBoxConstraints(BoolVector2D* exact_cover, int header) {
    for (int row = 0; row < SUDOKU_SIZE; row += BOX_SIZE)
        for (int column = 0; column < SUDOKU_SIZE; column += BOX_SIZE)
            for (int num = 0; num < SUDOKU_SIZE; num++, header++)
                markBoxConstraint(exact_cover, row, column, num, header);

    return header;
}

// We can place a number anywhere in the box, so mark each box cell.
void markBoxConstraint(BoolVector2D* exact_cover, int startRow, int startColumn,
                       int num, int header) {
    for (int box_row = 0; box_row < BOX_SIZE; box_row++)
        for (int box_column = 0; box_column < BOX_SIZE; box_column++) {
            int index = indexInExactCoverMatrix(startRow + box_row,
                                                startColumn + box_column, num);
            exact_cover->data[index]->data[header] = true;
        }
}

BoolVector2D* hexadokuToExactCover(uint8_t** hexadoku) {
    DEBUG_PRINTF("Converting hexadoku to exact cover...\n");
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
    DEBUG_PRINTF("Matrix width: %d, height: %d\n",
                 exact_cover->data[0]->capacity, exact_cover->capacity);

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
    DEBUG_PRINTF("Validating exact cover matrix...\n");
    // check that first row is filled with 1s
    for (int i = 0; i < exact_cover->data[0]->capacity; i++) {
        if (!exact_cover->data[0]->data[i]) {
            DEBUG_PRINTF("First row is not filled with 1s!\n");
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
            DEBUG_PRINTF("Column %d has no 1s!\n", i);
            return false;
        }
    }
    DEBUG_PRINTF("Exact cover validation complete!\n");
    return true;
}
