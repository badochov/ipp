CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -g
DEPS = commands.h BST.h input.h validator.h
OBJ = commands.o main.o input.o BST.o validator.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o forests $^ $(CFLAGS)

clean:
	rm -f *.o forests