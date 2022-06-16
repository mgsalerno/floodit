SHELL = /bin/bash
CC = gcc
CFLAGS = -g -Wall -O3

# ---------------------------------------

subdir = queue-graph/src
sources := $(shell $\
             gfl=$$(ls $(subdir)/{g,q}*.c); $\
             echo "$$gfl")
sources += $(wildcard *.c)
objects = $(sources:.c=.o)
exec = floodit

# ---------------------------------------

.PHONY: all clean purge

# ---------------------------------------

all: $(exec)

$(exec): $(objects)
	$(CC) $(CFLAGS) $^ -o $@

# ---------------------------------------

clean:
	rm -f $(objects)

purge:
	rm -f $(objects) $(exec)
