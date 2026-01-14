# Compiler & Flags

CC := gcc

# Warnings
WFLAGS := \
    -Wall -Wextra -Wpedantic -Werror \
    -Wshadow -Wconversion -Wsign-conversion \
    -Wformat=2 -Wformat-security \
    -Wnull-dereference -Wdouble-promotion \
    -Wstrict-prototypes -Wold-style-definition \
    -Wswitch-enum -Wswitch-default -Wfloat-equal \
    -Wundef -Wimplicit-fallthrough \
    -Wunused-parameter -Wmisleading-indentation \
    -Wduplicated-cond -Wduplicated-branches \
    -Wlogical-op -Wjump-misses-init \
    -Wpacked -Wredundant-decls

# Cast & aliasing
CAFLAGS := \
    -Wcast-qual -Wcast-align=strict \
    -Wstrict-aliasing=2

# Analyzer
AFLAGS := \
    -Wanalyzer-use-after-free

# Sanitizers
SANITIZERS := \
    -fsanitize=address,undefined,leak \
    -fsanitize-address-use-after-scope

# Security & hardening
SECFLAGS := \
    -fstack-protector-strong \
    -D_FORTIFY_SOURCE=2 \
    -fno-common

# Debug & standard
DBGFLAGS := \
    -std=c17 \
    -O0 -g -fno-omit-frame-pointer

CFLAGS := $(WFLAGS) $(CAFLAGS) $(AFLAGS) $(SANFLAGS) $(SECFLAGS) $(DBGFLAGS)

# Directories & Files

SRC_DIR   := src
BUILD_DIR := build
EXEC      := launcher

SOURCES   := $(shell find $(SRC_DIR) -type f -name '*.c')
HEADERS   := $(shell find $(SRC_DIR) -type f -name '*.h')
OBJECTS   := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# Targets

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJECTS) | $(BUILD_DIR) # build launcher
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(BUILD_DIR) # build files and objects
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

$(BUILD_DIR): # create new directory
	@mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(EXEC)
