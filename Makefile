CC=gcc
LIBS=-lSDL2 -llua -Iinclude/
NAME=juno8
FILES=src/*

juno:
	$(CC) $(FILES) -o $(NAME) $(LIBS) && ./juno8

run:
	./juno8

clean:
	rm juno8