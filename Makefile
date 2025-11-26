CC		:= gcc
CFLAGS 	:= -std=c17 -Wall -Wextra -Werror -Wshadow -Wconversion -pedantic -g

SRC_DIR 	:= src
BUILD_DIR 	:= build

EXEC 		:= launcher

SOURCES := $(shell find $(SRC_DIR) -name '*.c')
HEADERS := $(shell find $(SRC_DIR) -name '*.h')
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

all: $(BUILD_DIR) $(EXEC)

$(BUILD_DIR):
	@mkdir -p $@

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(EXEC)

.PHONY: all clean
