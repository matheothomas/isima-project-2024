CC=gcc
CFLAGS=-g -Wall -Wextra
SDLFLAGS=-lSDL2 -lSDL2_image -lSDL2_ttf -I/opt/local/include -L/opt/local/lib

all: exo1 exo2

exo1: ./X_fenetre/exo1.c
	${CC} ${CFLAGS} ./X_fenetre/exo1.c -o exo1 ${SDLFLAGS}

exo2: ./Snake/exo2.c
	${CC} ${CFLAGS} ./Snake/exo2.c -o exo2 ${SDLFLAGS}

clean:
	rm -f *.o *.gch