# Hexadoku solver 

This project is a CLI hexadoku puzzle solver, developed as part of the university coursework. It ranked first in speed compared to solutions from other students.

## About

Hexadoku is a 16x16 sudoku variant using hexadecimal digits (0-F) instead of the standard 1-9 in 9x9 grids. Input an unsolved puzzle and receive its solution. If the solution is not single, outputs the number of solutions. Check the `data` folder for input format examples. 

## Building the Project

### Dependencies

- C++17
- g++ (7.3 or later for full C++17 support)

### Build Commands

- `make all` - Build the project
- `make compile` - Compile the project
- `make test` - Test the compiled code with the test script
- `make clean` - Clean the project artifacts

## Getting Started

1. Clone the repository.
2. Ensure you have all the dependencies installed.
3. Build the project using `make all`.
4. Run the project tests using `make test`.

## Features

### Puzzle Input
Users can provide the unsolved hexadoku puzzle to the solver which will then process the input and generate the solution.

### Validation
The solver ensures that the provided puzzle has a valid structure and is solvable. If not, it notifies the user about the inconsistency in the puzzle.

### Error Handling
The tool has built-in error handling, making sure users are guided properly in cases of faulty inputs.

## Algorithm

Based on Knuth's Algorithm X with DLX, the solver is enhanced for speed. The process:

- Transforms hexadoku to exact cover problem.
- Starts with an empty set, selecting a constraint column.
- Picks a row from the column for the solution set.
- Covers columns satisfied by the row's 1s.
- Recurses through columns and rows for a solution.
- Backtracks if a row results in a dead end.
- Continues until a solution is found or all routes explored.
