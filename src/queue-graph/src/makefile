CC = gcc
CFLAGS = -g -Wall -Wextra -O3

source = $(wildcard *.c)
objects = $(source:.c=.o)

# ------------------------------------------------------------------------------

.PHONY: clean purge

main: $(objects)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(objects)

purge:
	rm -f $(objects) main