CC=gcc
CFLAGS=-g -Wall -Wextra
SDLFLAGS=-lSDL2 -lSDL2_image -lSDL2_ttf -I/opt/local/include -L/opt/local/lib
FLAG=-lm

exo3: ./animation.c
	${CC} ${CFLAGS} ./animation.c -o exo3 ${SDLFLAGS} ${FLAG}

clean:
	rm -f *.o *.gch
