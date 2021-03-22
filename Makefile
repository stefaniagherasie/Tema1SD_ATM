
CC = gcc
CFLAGS = -Wall -lm
SRC = atm.c
EXE = atm
 
build:
	$(CC) -o $(EXE) $(SRC) $(CFLAGS)

run: build
	./atm
.PHONY: clean
clean :
	rm -f $(EXE) *~