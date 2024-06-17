#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdio.h>

void destroyWindows(SDL_Window ** windows, int nombre) {
	for (int i = 0; i < nombre; i++) {
		SDL_DestroyWindow(windows[i]);
	}
}

void rotateWindows(SDL_Window ** windows, int centreX, int centreY, int nombre, float angle) {
	int x;
	int y;
	for (int i = 0; i < nombre; i++) {
		SDL_GetWindowPosition(windows[i], &x, &y);
		y = (int) (sinf(angle - i) * 200 + centreY);
		x = (int) (sinf(angle - i + M_PI/2) * 200 + centreX);
		//printf("Window %d angle : %f x : %d y : %d\n", i, sinf(angle)*1000, x, y);
		SDL_SetWindowPosition(windows[i], x, y);
	}
}

int main(int argc, char **argv) {


	int nombre = 7;
	SDL_DisplayMode mode;

	SDL_Window ** windows = (SDL_Window **) malloc(nombre*sizeof(SDL_Window *));

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Error : SDL initialisation - %s\n", 
		  SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_GetCurrentDisplayMode(0, &mode);
	int width = mode.w;
	int height = mode.h;

	int centreX = width / 2;
	int centreY = height / 2;


	for (int i = 0; i < nombre; i++) {
		windows[i] = SDL_CreateWindow("Fenêtre", centreX, (height/nombre)*i + 0, 200, 200, SDL_WINDOW_RESIZABLE);
		if (windows[i] == NULL) {
			SDL_Log("Error : SDL window %i creation - %s\n", i, SDL_GetError());
			for (int j = 0; j < nombre; j++) {
				SDL_DestroyWindow(windows[j]);
			}
			SDL_Quit();
			exit(EXIT_FAILURE);
		}
	}

	SDL_Event event;
	float angle = 0.; //degrés
	
	while (1) {
		
		angle += 0.04;

		rotateWindows(windows, centreX, centreY, nombre, angle);
		
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_WINDOWEVENT:
					switch (event.window.event) {

						case SDL_WINDOWEVENT_CLOSE:
							destroyWindows(windows, nombre);
							SDL_Quit();
							return EXIT_FAILURE;
							break;
					
						case SDL_WINDOWEVENT_SIZE_CHANGED:
						break;
					}
				break;
				
				case SDL_QUIT:
					destroyWindows(windows, nombre);
					SDL_Quit();
					return EXIT_FAILURE;
					break;
			}
		}
		SDL_Delay(1);
	}

	SDL_Delay(2000);

	destroyWindows(windows, nombre);
	
	SDL_Quit();

	return 0;
}
