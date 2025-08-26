CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -O2
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
EXEC = launcher

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(EXEC)
