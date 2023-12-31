CC = gcc

SOURCES = $(wildcard *.c)
OBJECTS = $(subst .c,.o,$(SOURCES))

.PHONY: clean

all : coolness 

coolness : $(OBJECTS)
	$(CC) -o coolness $(OBJECTS) -lm

%.o : %.c
	$(CC) -c $<

clean:
	rm -f coolness $(OBJECTS)

