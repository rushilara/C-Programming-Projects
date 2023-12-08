CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lcurl 
OBJ = crawler.o curl.o set.o hashtable.o url.o pagedir.o
DEPS = crawler.h curl.h set.h hashtable.h url.h pagedir.h
EXEC = crawler

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) $(EXEC)

