#CFLAGS = -O -Wall -Wextra -O2
#
#OBJ =\
#    pierwszy.o \
#    drugi.o \
#    trzeci.o \
#    czwarty.o
#
#all: main
#
#clean:
#       rm -f *.o test
#.c.o:
#       $(CC) -c $(INCLUDES) $(CFLAGS) $<
#
#main: $(OBJ)
#       $(CC) $(OBJ) -o test

CC = gcc

all: main.c BST.c forests.c input.c
	$(CC) BST.c forests.c main.c input.c -o lasy -I . -Wall -Wextra -O2 -g

#main.o: main.c
#	$(CC) main.c -c -o main.o
#
#BST.o: BST.c BST.h
#	$(CC) BST.c -c -o BST.o
#
#forests.o: forests.c forests.h
#	$(CC) forests.c -c -o forests.o

clean:
	rm -f *.o lasy