CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Werror -pedantic -g -I$(SRC_DIR)
SRC_DIR = src
TEST_DIR = tests
SOURCES = $(shell find $(SRC_DIR) -name '*.c')
HEADERS = $(shell find $(SRC_DIR) -name '*.h')
TEST_SOURCES = $(shell find $(TEST_DIR) -name '*.c')
OBJECTS = $(SOURCES:.c=.o)
MAIN_SRC = $(SRC_DIR)/main.c
MAIN_OBJ = $(MAIN_SRC:.c=.o)
EXEC = launcher
TEST_EXECS = $(patsubst $(TEST_DIR)/%.c, $(TEST_DIR)/%, $(TEST_SOURCES))

# Default target: build main program (runs all tests)
all: $(EXEC)

# Main program: link all objects
$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Test target: build and run separate test executables
test: $(TEST_EXECS)
	@for test in $(TEST_EXECS); do \
		./$$test; \
	done

# Test executables: link each test .c with corresponding src/ .c
$(TEST_DIR)/test_%: $(TEST_DIR)/test_%.o $(SRC_DIR)/%.o
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c to .o, depend on all headers
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(SRC_DIR)/*.o $(TEST_DIR)/*.o $(EXEC) $(TEST_EXECS)

.PHONY: all test clean
