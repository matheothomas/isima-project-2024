/* name : main.c
 * author : matheo thomas
 * date : 16-06-24
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "SDL2/SDL.h"

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

/// Draws a rectangle at the defined coordinates, with the defined size and angle.
void drawRect(SDL_Renderer *renderer, int x, int y, int w, int h, int angle) {

}

void draw(SDL_Renderer* renderer) {
	SDL_Rect rect;

	SDL_SetRenderDrawColor(renderer,                                       
						50, 0, 0,                             // mode Red, Green, Blue (tous dans 0..255)
						255);                                 // 0 = transparent ; 255 = opaque
	rect.x = 0;                                             // x haut gauche du rectangle
	rect.y = 0;                                                  // y haut gauche du rectangle
	rect.w = 400;                                                // sa largeur (w = width)
	rect.h = 400;                                                // sa hauteur (h = height)

	SDL_RenderFillRect(renderer, &rect);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);                   
	SDL_RenderDrawLine(renderer,                             
					0, 0,                                          // x,y du point de la première extrémité
					400, 400);                                // x,y seconde extrémité


	/* tracer un cercle n'est en fait pas trivial, voilà le résultat sans algo intelligent ... */
	for (float angle = 0; angle < 2 * M_PI; angle += M_PI / 4000) {      
		SDL_SetRenderDrawColor(renderer,
						 (cos(angle * 2) + 1) * 255 / 2,          // quantité de Rouge      
						 (cos(angle * 5) + 1) * 255 / 2,          //          de vert 
						 (cos(angle) + 1) * 255 / 2,              //          de bleu
		255);                                    // opacité = opaque
		SDL_RenderDrawPoint(renderer,                  
					  200 + 100 * cos(angle),                     // coordonnée en x
					  200 + 150 * sin(angle));                    //            en y   
	}
}

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

	/*********************************************************************************************************************/
	/*                                     On dessine dans le renderer                                                   */
	/*********************************************************************************************************************/
	/*             Cette partie pourrait avantageusement être remplacée par la boucle évènementielle                     */ 
	draw(renderer);                                      // appel de la fonction qui crée l'image  
	SDL_RenderPresent(renderer);                         // affichage

	SDL_Event event;

	int running = 1;

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
							// showWindow(renderer, laby, SIZE);
						break;

					}
				break;
				// case SDL_MOUSEBUTTONDOWN:
				// printf("Mouse button down.\n");

				// break;
				case SDL_QUIT:
					printf("Quit.\n");
				running = 0;
			}
		}
		SDL_Delay(1);
	}

	/* on referme proprement la SDL */
	end_sdl(1, "Normal ending", window, renderer);
	return EXIT_SUCCESS;
}
