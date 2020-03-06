CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -g
DEPS = forests.h BST.h input.h
OBJ = forests.o main.o input.o BST.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o forests $^ $(CFLAGS)

clean:
	rm -f *.o forests