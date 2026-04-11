COMPILER := gcc

# default build
BUILD ?= debug

# directories
SOURCE_DIR := src
INCLUDE_DIR := include

BUILD_DIR := build/$(BUILD)
OBJECT_DIR := $(BUILD_DIR)/obj
BINARY_DIR := $(BUILD_DIR)/bin

TARGET_EXECUTABLE := launcher
BINARIES := $(BINARY_DIR)/$(TARGET_EXECUTABLE)

STANDARD := -std=c17
INCLUDES := -I$(INCLUDE_DIR)

# dependency flags for .h files
# generate new .o files without repeat build generation
# -MMD generate dependency file (.d)
# -MP safety from deletion failure
DEPENDENCY_FLAGS := -MMD -MP

WARNINGS := \
	-Wall -Werror -Wextra -Wpedantic -Wshadow \
	-Wunused-value \
	-Wunused-variable \
	-Wunused-parameter

BASE_FLAGS := \
	$(STANDARD) \
	$(WARNINGS)

CFLAGS := \
	$(BASE_FLAGS) \
	$(DEPENDENCY_FLAGS)

ifeq ($(BUILD), debug)
	CFLAGS += \
		-DDEBUG \
		-O0 \
		-g3
else ifeq ($(BUILD), release)
	CFLAGS += \
		-DNDEBUG \
		-O3 \
		-g0
# default
else
$(error Unknown BUILD=$(BUILD))
endif

SOURCES := main.c $(shell find $(SOURCE_DIR) -name '*.c')
OBJECTS := $(patsubst %.c,$(OBJECT_DIR)/%.o,$(SOURCES))

all: $(BINARIES)

# compilation rule
$(BINARIES): $(OBJECTS)
	@mkdir -p $(BINARY_DIR)
	$(COMPILER) $^ -o $@

# linkage
$(OBJECT_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(COMPILER) $(CFLAGS) $(INCLUDES) -c $< -o $@

# include dependencies
-include $(OBJECTS:.o=.d)

run: all
	./$(BINARIES)

clean:
	rm -rf build

rebuild: clean all

.PHONY: all clean rebuild
