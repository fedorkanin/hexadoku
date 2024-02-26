CC = clang
COMMON_FLAGS = -Wall -pedantic -Iinclude -std=c17

# Add profiling and coverage flags for the development build
CFLAGS_DEV ?= $(COMMON_FLAGS) -fsanitize=address -g -fprofile-instr-generate -fcoverage-mapping
CFLAGS_RELEASE ?= $(COMMON_FLAGS) -O3
LDFLAGS_DEV = -fsanitize=address -fprofile-instr-generate -fcoverage-mapping
LDFLAGS_RELEASE =

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_SCRIPT = test.sh

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES_DEV = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/dev/%.o)
OBJ_FILES_RELEASE = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/release/%.o)

DEP_FILES_DEV = $(OBJ_FILES_DEV:.o=.d)
DEP_FILES_RELEASE = $(OBJ_FILES_RELEASE:.o=.d)

TARGET_DEV = $(BIN_DIR)/main_dev.out
TARGET_RELEASE = $(BIN_DIR)/main_release.out

.PHONY: all clean test dev release profile

all: dev

dev: CFLAGS = $(CFLAGS_DEV)
dev: LDFLAGS = $(LDFLAGS_DEV)
dev: $(TARGET_DEV)

release: CFLAGS = $(CFLAGS_RELEASE)
release: LDFLAGS = $(LDFLAGS_RELEASE)
release: $(TARGET_RELEASE)

-include $(DEP_FILES_DEV) $(DEP_FILES_RELEASE)

$(TARGET_DEV): $(OBJ_FILES_DEV) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET_RELEASE): $(OBJ_FILES_RELEASE) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/dev/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)/dev
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR)/release/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)/release
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR)/dev $(OBJ_DIR)/release $(BIN_DIR):
	mkdir -p $@

test: dev
	./$(TEST_SCRIPT)

profile: dev
	llvm-profdata merge -sparse default.profraw -o default.profdata
	llvm-cov show ./$(TARGET_DEV) -instr-profile=default.profdata

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) default.profraw default.profdata