/* name : main.c
 * author : matheo thomas
 * date : 16-06-24
 */

#include "SDL2/SDL_error.h"
#include "SDL2/SDL_log.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_video.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


int main(void){
	int n;
	SDL_DisplayMode dm;
	int w, h;
	int x, y, cx, cy, m;

	// Number of windows.
	n = 32;

	// Create SDL array of n windows
	SDL_Window **window = (SDL_Window **)malloc(n*sizeof(SDL_Window *));

	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_Delay(500);

	// Create and position each window
	SDL_GetCurrentDisplayMode(0, &dm);
	w = dm.w;
	h = dm.h;

	for(int i = 0; i < n; i += 2) {
		window[i] = SDL_CreateWindow("Window", w/5 + (w/50)*i, h/5 + (h/50)*i, 50, 50, SDL_WINDOW_RESIZABLE);

		if(window[i] == NULL) {
			SDL_Log("Error : SDL window creation - %s\n", SDL_GetError());
			SDL_Quit();
			exit(EXIT_FAILURE);
		}
		window[i+1] = SDL_CreateWindow("Window", w/5 + (w/50)*i, h - h/5 - (h/50)*i, 50, 50, SDL_WINDOW_RESIZABLE);

		if(window[i+1] == NULL) {
			SDL_Log("Error : SDL window creation - %s\n", SDL_GetError());
			SDL_Quit();
			exit(EXIT_FAILURE);
		}
		SDL_Delay(50);
	}

	// Moving the windows to make the X rotate
	cx = w/2;
	cy = h/2;

	// number of movements to achieve the rotation
	m = 10;

	int **tab = (int **)malloc(n*sizeof(int *));
	for( int i = 0; i < n; i++) {
		tab[i] = (int *)malloc(2*sizeof(int));
	}
	for(int j = 0; j < n; j += 2) {
		SDL_GetWindowPosition(window[j], &x, &y);
		tab[j][0] = (cx - x)/m;
		tab[j][1] = (cy - y)/m;
	}

	for(int i = 0; i < 2*m; i++) {
		for(int j = 0; j < n; j += 2) {
			if(j < n/2) {
				SDL_GetWindowPosition(window[j], &x, &y);
				SDL_SetWindowPosition(window[j], x + tab[j][0], y);
				SDL_GetWindowPosition(window[j+1], &x, &y);
				SDL_SetWindowPosition(window[j+1], x, y - tab[j][1]);
			} else {
				SDL_GetWindowPosition(window[j], &x, &y);
				SDL_SetWindowPosition(window[j], x + tab[j][0], y);
				SDL_GetWindowPosition(window[j+1], &x, &y);
				SDL_SetWindowPosition(window[j+1], x, y - tab[j][1]);
			}
		}
		SDL_Delay(10);
	}

	SDL_Delay(2000);

	// Freeing resources
	for(int i = 0; i < n; i++) {
		free(tab[i]);
	}
	free(tab);
	
	for(int i = 0; i < n; i++) {
		SDL_DestroyWindow(window[i]);
	}

	free(window);

	SDL_Quit();

	return 0;
}

