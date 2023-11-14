CC=gcc
SOURCE=test_ll_cycle.c ll_cycle.c
OBJ=test_ll_cycle.o ll_cycle.o
HEADER=ll_cycle.h
CFLAGS=-Wall -Wextra -Wpedantic

check_cycle: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $@

%.o: %.c $(HEADER)
	$(CC) $< -c

clean:
	rm -rf check_cycle
	rm -rf *.o
