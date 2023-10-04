CC = gcc
CFLAGS = -Wall -pedantic -Iinclude -O2 -std=c18 -MMD -MP
DEBUG_FLAGS = -g -DDEBUG
PREGEN_FLAGS = -DUSE_PREGEN_ARRAY

SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_DIR = obj
OBJ_DIR_PREGEN = obj-pregen
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
OBJ_FILES_PREGEN = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR_PREGEN)/%.o,$(SRC_FILES))

BIN_DIR = bin
TARGET = $(BIN_DIR)/main.out
TARGET_PREGEN = $(BIN_DIR)/main-pregen.out

TEST_SCRIPT = test.sh

.PHONY: all clean test compile debug test-all

all: compile
compile: $(TARGET)

$(TARGET): $(OBJ_FILES) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TARGET_PREGEN): $(OBJ_FILES_PREGEN) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(PREGEN_FLAGS) -o $@ $^

$(OBJ_DIR_PREGEN)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR_PREGEN)
	$(CC) $(CFLAGS) $(PREGEN_FLAGS) -c -o $@ $<

$(OBJ_DIR_PREGEN):
	mkdir -p $(OBJ_DIR_PREGEN)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

test: compile
	./$(TEST_SCRIPT)

test-all: $(TARGET) $(TARGET_PREGEN)
	./$(TEST_SCRIPT) $(TARGET) # Test with dynamically generated DLX
	./$(TEST_SCRIPT) $(TARGET_PREGEN) # Test with pregenerated DLX

clean:
	rm -rf $(OBJ_DIR) $(OBJ_DIR_PREGEN) $(BIN_DIR) Hexadoku.dSYM

-include $(OBJ_FILES:.o=.d)
-include $(OBJ_FILES_PREGEN:.o=.d)
