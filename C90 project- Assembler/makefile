CC = gcc
CFLAGS = -Wall -std=c90 -pedantic -ansi -g -I/home/user/Desktop/Project14Assembler/headerfiles
SRCDIR = /home/user/Desktop/Project14Assembler

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = main

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

