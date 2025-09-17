CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Werror -pedantic -g -fsanitize=address -I$(SRC_DIR)
SRC_DIR = src
SOURCES = $(shell find $(SRC_DIR) -name '*.c')
HEADERS = $(shell find $(SRC_DIR) -name '*.h')
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
EXEC = launcher

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(SRC_DIR)/*.o $(SRC_DIR)/*.o $(EXEC)

.PHONY: all clean
