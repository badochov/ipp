CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
DEPS = commands.h BST.h input.h validator.h util.h
OBJ = commands.o main.o input.o BST.o validator.o util.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o forests $^ $(CFLAGS)

clean:
	rm -f *.o forests