CC=gcc
SOURCE=test_growing_arrays.c growing_arrays.c
OBJ=test_growing_arrays.o growing_arrays.o
HEADER=growing_arrays.h
CFLAGS=-Wall -Wextra -Wpedantic

growing_arrays: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $@

%.o: %.c $(HEADER)
	$(CC) $< -c

clean:
	rm -rf growing_arrays
	rm -rf *.o
