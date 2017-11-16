CC=gcc
LIBS=-lSDL2 -Iinclude -Llib -llua -lm -ldl
NAME=juno8
FILES=src/*.c

juno:
	$(CC) $(FILES) -o $(NAME) $(LIBS)

run:
	./juno8

clean:
	rm juno8