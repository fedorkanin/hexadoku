CC = g++
CFLAGS = -Wall -pedantic -Iinclude -O2 -std=c++17 -MMD -MP
DEBUG_FLAGS = -g -DDEBUG

SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_DIR = obj
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

BIN_DIR = bin
TARGET = $(BIN_DIR)/main.out

TEST_SCRIPT = test.sh

.PHONY: all clean test compile debug

all: compile
compile: $(TARGET)

$(TARGET): $(OBJ_FILES) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

test: compile
	./$(TEST_SCRIPT)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) Hexadoku.dSYM

-include $(OBJ_FILES:.o=.d)
