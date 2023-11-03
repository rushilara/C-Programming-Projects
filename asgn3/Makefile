CC=gcc
SOURCE=test_password_checker.c password_checker.c
OBJ=test_password_checker.o password_checker.o
HEADER=password_checker.h

password_checker: $(OBJ)
	$(CC) $(OBJ) -o $@

%.o: %.c $(HEADER)
	$(CC) $< -c

clean:
	rm -rf password_checker
	rm -rf *.o

