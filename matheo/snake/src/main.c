/* name : main.c
 * author : matheo thomas
 * date : 16-06-24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include "../include/main.h"

void end_sdl(char ok,                                               // fin normale : ok = 0 ; anormale ok = 1
			 char const *msg,                                       // message à afficher
			 SDL_Window *window,                                    // fenêtre à fermer
			 SDL_Renderer *renderer) {                              // renderer à fermer
	char msg_formated[255];                                            
	int l;                                                     

	if (!ok) {                                                        // Affichage de ce qui ne va pas
		strncpy(msg_formated, msg, 250);                                         
		l = strlen(msg_formated);                                            
		strcpy(msg_formated + l, " : %s\n");                                     

		SDL_Log(msg_formated, SDL_GetError());                                   
	}                                                          

	if (renderer != NULL) {                                           // Destruction si nécessaire du renderer
		SDL_DestroyRenderer(renderer);                                  // Attention : on suppose que les NULL sont maintenus !!
		renderer = NULL;
	}
	if (window != NULL)   {                                           // Destruction si nécessaire de la fenêtre
		SDL_DestroyWindow(window);                                      // Attention : on suppose que les NULL sont maintenus !!
		window= NULL;
	}

	SDL_Quit();                                                    

	if (!ok) {                                       // On quitte si cela ne va pas            
		exit(EXIT_FAILURE);                                                  
	}                                                          
}                                                        

void drawRect(SDL_Renderer *renderer, rect_t r) {
	int x1, y1, x2, y2;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// calculates rectangle sides coordinates
	x1 = r.x + r.w*cos(r.angle*M_PI/180);
	y1 = r.y + r.w*sin(r.angle*M_PI/180);
	x2 = r.x + r.h*cos((r.angle+90)*M_PI/180);
	y2 = r.y + r.h*sin((r.angle+90)*M_PI/180);

	// traces the rectangle
	SDL_RenderDrawLine(renderer, r.x, r.y, x1, y1);
	SDL_RenderDrawLine(renderer, r.x, r.y, x2, y2);
	SDL_RenderDrawLine(renderer, x2, y2, x1+x2-r.x, y1+y2-r.y);
	SDL_RenderDrawLine(renderer, x1, y1, x1+x2-r.x, y1+y2-r.y);
}

rect_t createRect(SDL_Window *window, int w, int h) {
	rect_t r;

	SDL_GetWindowSize(window, &r.x, &r.y);

	r.x = r.x/2;
	r.y = r.y/2;
	r.w = w;
	r.h = h;
	r.angle = 0;
	r.vx = rand() % 8;
	r.vy = rand() % 8;

	if(rand() % 2) {
		r.vx = -r.vx;
	}
	if(rand() % 2) {
		r.vy = -r.vy;
	}

	return r;
}

int main(int argc, char **argv) {
	int n, m, running, j, rotate,
		xClick, yClick,
		windowWidth, windowHeight;

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_DisplayMode screen;
	SDL_Event event;

	srand(time(0));

	// number of rectangles
	if(argc == 2) {
		n = atoi(argv[1]);
	} else {
		printf("To choose the number of rectangles, use %s <number of rectangles>\n", argv[0]);
		n = 3;
	}

	m = 0;
	running = 1;
	j = 0;
	rotate = 1;


	// SDL INITIALISATION
	if (SDL_Init(SDL_INIT_VIDEO) != 0) end_sdl(0, "ERROR SDL INIT", window, renderer);

	SDL_GetCurrentDisplayMode(0, &screen);

	// Window creation
	window = SDL_CreateWindow("Premier dessin",
						   SDL_WINDOWPOS_CENTERED,
						   SDL_WINDOWPOS_CENTERED, screen.w * 0.76,
						   screen.h * 0.76,
	SDL_WINDOW_OPENGL);
	if (window == NULL) end_sdl(0, "ERROR WINDOW CREATION", window, renderer);

	// Renderer creation
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) end_sdl(0, "ERROR RENDERER CREATION", window, renderer);


	// Rectangles creation
	rect_t *rect_tab = (rect_t *)malloc(n*sizeof(rect_t));
	for(int i = 0; i < n; i++) {
		rect_tab[i] = createRect(window, 50, 50);
	}


	// SDL main loop
	while(running){
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_WINDOWEVENT:
					switch (event.window.event){
						case SDL_WINDOWEVENT_CLOSE:
						break;
					}
				break;
				case SDL_MOUSEBUTTONDOWN:
					xClick = event.button.x;
					yClick = event.button.y;
					for(int i = 0; i < m; i++) {
						if(xClick > rect_tab[i].x && 
							xClick < rect_tab[i].x + rect_tab[i].w && 
							yClick > rect_tab[i].y &&
							yClick < rect_tab[i].y + rect_tab[i].h) {
							rect_tab[i].vx = -(rect_tab[i].vx + rand() % 5);
							rect_tab[i].vy = -(rect_tab[i].vy + rand() % 5);

						}
					}
				break;
				case SDL_QUIT:
				running = 0;
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		for(int i = 0; i < m; i++) {
			drawRect(renderer, rect_tab[i]);

			rect_tab[i].x += rect_tab[i].vx;
			rect_tab[i].y += rect_tab[i].vy;
			if(rect_tab[i].angle > 20) {
				rotate = -1;
				rect_tab[i].angle += rotate;
			} else if (rect_tab[i].angle < -20) {
				rotate = 1;
				rect_tab[i].angle += rotate;
			} else {
				rect_tab[i].angle += rotate;
			}
		}

		SDL_RenderPresent(renderer);

		SDL_GetWindowSize(window, &windowWidth, &windowHeight);

		for(int i = 0; i < m; i++) {
			if(rect_tab[i].x + rect_tab[i].w > windowWidth || 
				rect_tab[i].y + rect_tab[i].h > windowHeight ||
				rect_tab[i].x < 0 ||
				rect_tab[i].y < 0) {
				running = 0;
			}
		}

		if(m < n) {
			j++;
		}

		if(j >= 20 && m < n) {
			j = 0;
			m++;
		}

		SDL_Delay(50);
	}

	// SDL closing
	end_sdl(1, "Normal ending", window, renderer);
	return EXIT_SUCCESS;
}
