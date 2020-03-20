CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
DEPS = commands.h BST.h input.h validator.h util.h
OBJ = commands.o main.o input.o BST.o validator.o util.o
ZIP_NAME = male_zadanie_Hubert_Badocha

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o forests $^ $(CFLAGS)

clean:
	rm -f ./*.o forests

zip:
	([[ -f ./$(ZIP_NAME).zip ]] && rm ./$(ZIP_NAME).zip) & find .  -maxdepth 1 -name "*.[ch]" | zip $(ZIP_NAME) Makefile test.sh -@
