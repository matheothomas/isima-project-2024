#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "../include/main.h"

void end_sdl(char ok, char const* msg, SDL_Window* window, SDL_Renderer* renderer) {
	char msg_formated[255];                                            
	int l;                                                     

	if (!ok) {
		strncpy(msg_formated, msg, 250);                                         
		l = strlen(msg_formated);                                            
		strcpy(msg_formated + l, " : %s\n");                                     

		SDL_Log(msg_formated, SDL_GetError());                                   
	}                                                          

	if (renderer != NULL) {
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
	if (window != NULL)   {
		SDL_DestroyWindow(window);
		window= NULL;
	}

	SDL_Quit();                                                    

	if (!ok) { 
		exit(EXIT_FAILURE);                                                  
	}                                                          
}                                                        

void draw(SDL_Renderer* renderer) {
	SDL_Rect rectangle;                                                

	SDL_SetRenderDrawColor(renderer,                                       
						50, 0, 0, 255);
	rectangle.x = 0;
	rectangle.y = 0;
	rectangle.w = 400;
	rectangle.h = 400;

	SDL_RenderFillRect(renderer, &rectangle);                        

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);                   
	SDL_RenderDrawLine(renderer,                             
					0, 0, 400, 400);

	Rect recttest;
	Point p1;
	Point p2;
	p1.x = 10;
	p1.y = 10;
	p2.x = 100;
	p2.y = 200;
	recttest.p1 = &p1;
	recttest.p2 = &p2;
	recttest.w = 50;
	drawRectangle(renderer, recttest);
}

void drawRectangle(SDL_Renderer * renderer, Rect rectangle) {
	SDL_RenderDrawLine(renderer, rectangle.p1 -> x, rectangle.p1 -> y, rectangle.p2 -> x, rectangle.p2 -> y);
	float angle = atan2f((float) (rectangle.p1 -> y - rectangle.p2 -> y), (float)(rectangle.p2 -> x - rectangle.p1 -> x));
	int rayon = rectangle.w / 2;
	int x_offset = (int) (rayon * cosf(angle + M_PI/2));
	int y_offset = - (int) (rayon * sinf(angle + M_PI/2));
	int x1 = rectangle.p1 -> x + x_offset;
	int y1 = rectangle.p1 -> y + y_offset;
	int x4 = rectangle.p1 -> x - x_offset;
	int y4 = rectangle.p1 -> y - y_offset;
	int x2 = rectangle.p2 -> x + x_offset;
	int y2 = rectangle.p2 -> y + y_offset;
	int x3 = rectangle.p2 -> x - x_offset;
	int y3 = rectangle.p2 -> y - y_offset;
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
	SDL_RenderDrawLine(renderer, x3, y3, x4, y4);
	SDL_RenderDrawLine(renderer, x4, y4, x1, y1);
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Event event;

	SDL_DisplayMode screen;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) end_sdl(0, "ERROR SDL INIT", window, renderer);

	SDL_GetCurrentDisplayMode(0, &screen);
	printf("Résolution écran\n\tw : %d\n\th : %d\n",
		screen.w, screen.h);

	/* Création de la fenêtre */
	window = SDL_CreateWindow("Premier dessin",
						   SDL_WINDOWPOS_CENTERED,
						   SDL_WINDOWPOS_CENTERED, screen.w * 0.66,
						   screen.h * 0.66,
	SDL_WINDOW_OPENGL);
	if (window == NULL) end_sdl(0, "ERROR WINDOW CREATION", window, renderer);

	/* Création du renderer */
	renderer = SDL_CreateRenderer(window, -1,
							   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) end_sdl(0, "ERROR RENDERER CREATION", window, renderer);


	while (1) {
		draw(renderer);
		SDL_RenderPresent(renderer);
		
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_WINDOWEVENT:
					switch (event.window.event) {

						case SDL_WINDOWEVENT_CLOSE:
							end_sdl(1, "Normal ending", window, renderer);
							return EXIT_SUCCESS;
							break;
					
						case SDL_WINDOWEVENT_SIZE_CHANGED:
						break;
					}
				break;
				
				case SDL_QUIT:
					end_sdl(1, "Normal ending", window, renderer);
					return EXIT_SUCCESS;
					break;
			}
			SDL_Delay(1);
		}

	}
	end_sdl(1, "Normal ending", window, renderer);
	return EXIT_SUCCESS;
}
