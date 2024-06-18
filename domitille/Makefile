CC=gcc
CFLAGS=-g -Wall -Wextra
SDLFLAGS=-lSDL2 -lSDL2_image -lSDL2_ttf -I/opt/local/include -L/opt/local/lib
FLAG=-lm

all: exo1 exo2

exo1: ./X_fenetre/exo1.c
	${CC} ${CFLAGS} ./X_fenetre/exo1.c -o exo1 ${SDLFLAGS}

exo2: ./Snake/exo2bis.c
	${CC} ${CFLAGS} ./Snake/exo2bis.c -o exo2 ${SDLFLAGS}

clean:
	rm -f *.o *.gch
