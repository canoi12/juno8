CC=gcc
LIBS=-lSDL2 -llua -Iinclude/
NAME=juno8
FILES=src/*

juno:
	$(CC) $(FILES) -o $(NAME) $(LIBS)