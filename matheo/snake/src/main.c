/* name : main.c
 * author : matheo thomas
 * date : 16-06-24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_video.h"
#include "../include/main.h"

/*********************************************************************************************************************/
/*                              Programme d'exemple de création de rendu + dessin                                    */
/*********************************************************************************************************************/
void end_sdl(char ok,                                               // fin normale : ok = 0 ; anormale ok = 1
			 char const* msg,                                       // message à afficher
			 SDL_Window* window,                                    // fenêtre à fermer
			 SDL_Renderer* renderer) {                              // renderer à fermer
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

void drawRect(SDL_Renderer *renderer, rect r) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	int x1, y1, x2, y2;
	x1 = r.x + r.w*cos(r.angle*M_PI/180);
	y1 = r.y + r.w*sin(r.angle*M_PI/180);
	x2 = r.x + r.h*cos((r.angle+90)*M_PI/180);
	y2 = r.y + r.h*sin((r.angle+90)*M_PI/180);
	SDL_RenderDrawLine(renderer, r.x, r.y, x1, y1);
	SDL_RenderDrawLine(renderer, r.x, r.y, x2, y2);
	SDL_RenderDrawLine(renderer, x2, y2, x1+x2-r.x, y1+y2-r.y);
	SDL_RenderDrawLine(renderer, x1, y1, x1+x2-r.x, y1+y2-r.y);
	

}

rect createRect(SDL_Window *window, int w, int h) {
	rect r;
	SDL_GetWindowSize(window, &r.x, &r.y);
	r.x = r.x/2;
	r.y = r.y/2;
	r.w = w;
	r.h = h;
	r.angle = 0;


	return r;
}

// void draw(SDL_Renderer* renderer) {
//
	// SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);                   
//
// }

int main(void) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	SDL_DisplayMode screen;

	/*********************************************************************************************************************/  
	/*                         Initialisation de la SDL  + gestion de l'échec possible                                   */
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


	SDL_Event event;

	int running = 1;
	int i = 0;
	int vx, vy;
	int xClick, yClick;
	int windowWidth, windowHeight;

	// rect r;
	// SDL_GetWindowSize(window, &r.x, &r.y);
	// r.x = r.x/2;
	// r.y = r.y/2;
	// r.w = 80;
	// r.h = 80;
	// r.angle = 0;
	
	rect r = createRect(window, 80, 80);

	srand(time(0));
	vx = rand() % 10;
	vy = rand() % 10;
	if(rand() % 2) {
		vx = -vx;
	}
	if(rand() % 2) {
		vy = -vy;
	}


	while(running){
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_WINDOWEVENT:
					switch (event.window.event){
						case SDL_WINDOWEVENT_CLOSE:
							printf("Window closed.\n");
							break;
							// case SDL_WINDOWEVENT_SIZE_CHANGED:
							// width = event.window.data1;
							// height = event.window.data2;
							// printf("Size : %d%d\n", width, height);
						break;
						default:
						break;

					}
				break;
				case SDL_MOUSEBUTTONDOWN:
					// printf("Mouse button down.\n");
					xClick = event.button.x;
					yClick = event.button.y;
					if(xClick > r.x && 
						xClick < r.x + r.w && 
						yClick > r.y &&
						yClick < r.y + r.h) {
						vx = -vx;
						vy = -vy;
					}

				break;
				case SDL_QUIT:
					printf("Quit.\n");
				running = 0;
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		drawRect(renderer, r);
		SDL_RenderPresent(renderer);
		r.x += vx;
		r.y += vy;
		SDL_Delay(50);
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
		if(r.x + r.w > windowWidth || 
			r.y + r.h > windowHeight ||
			r.x < 0 ||
			r.y < 0) {
			running = 0;
		}
	}

	/* on referme proprement la SDL */
	end_sdl(1, "Normal ending", window, renderer);
	return EXIT_SUCCESS;
}
