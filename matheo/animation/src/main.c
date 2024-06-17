/* name : main.c
 * author : matheo thomas
 * date : 17-06-24
 */

#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

void end_sdl(char ok,                               // fin normale : ok = 0 ; anormale ok = 1
			 char const *msg,                       // message à afficher
			 SDL_Window *window,                    // fenêtre à fermer
			 SDL_Renderer *renderer) {              // renderer à fermer
	char msg_formated[255];                                            
	int l;                                                     

	if (!ok) {										// Affichage de ce qui ne va pas
		strncpy(msg_formated, msg, 250);                                         
		l = strlen(msg_formated);                                            
		strcpy(msg_formated + l, " : %s\n");                                     

		SDL_Log(msg_formated, SDL_GetError());                                   
	}                                                          

	if (renderer != NULL) {							// Destruction si nécessaire du renderer
		SDL_DestroyRenderer(renderer);              // Attention : on suppose que les NULL sont maintenus !!
		renderer = NULL;
	}
	if (window != NULL)   {                         // Destruction si nécessaire de la fenêtre
		SDL_DestroyWindow(window);                  // Attention : on suppose que les NULL sont maintenus !!
		window= NULL;
	}

	SDL_Quit();                                                    

	if (!ok) {										// On quitte si cela ne va pas            
		exit(EXIT_FAILURE);                                                  
	}                                                          
}

SDL_Texture* load_texture_from_image(char  *  file_image_name, SDL_Window *window, SDL_Renderer *renderer ){
	SDL_Surface *my_image = NULL;           // Variable de passage
	SDL_Texture* my_texture = NULL;         // La texture

	my_image = IMG_Load(file_image_name);   // Chargement de l'image dans la surface
	if (my_image == NULL) end_sdl(0, "Chargement de l'image impossible", window, renderer);

	my_texture = SDL_CreateTextureFromSurface(renderer, my_image); // Chargement de l'image de la surface vers la texture
	SDL_FreeSurface(my_image);                                     // la SDL_Surface ne sert que comme élément transitoire 
	if (my_texture == NULL) end_sdl(0, "Echec de la transformation de la surface en texture", window, renderer);

	return my_texture;
}

void play_with_texture_2(SDL_Texture* my_texture, SDL_Window* window, SDL_Renderer* renderer, float i, int j) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect 
	source = {0},                      // Rectangle définissant la zone de la texture à récupérer
	window_dimensions = {0},           // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
	destination = {0};                 // Rectangle définissant où la zone_source doit être déposée dans le renderer
	
	SDL_Point point;

	SDL_GetWindowSize(
		window, &window_dimensions.w,
		&window_dimensions.h);               // Récupération des dimensions de la fenêtre
	SDL_QueryTexture(my_texture, NULL, NULL,
				  &source.w, &source.h);  // Récupération des dimensions de l'image

	float zoom = 1.2 + 0.01*i;                        // Facteur de zoom à appliquer    
	destination.w = source.w * zoom;         // La destination est un zoom de la source
	destination.h = source.h * zoom;         // La destination est un zoom de la source
	destination.x =
		(window_dimensions.w - destination.w*0.8) ;     // La destination est au milieu de la largeur de la fenêtre
	destination.y =
		(window_dimensions.h - destination.h*0.9) ;  // La destination est au milieu de la hauteur de la fenêtre
	
	point.x = source.w;
	point.y = source.h;

		SDL_RenderCopyEx(renderer, my_texture, &source, &destination, j*0.1, &point, SDL_FLIP_NONE);

}


int main(void){
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_DisplayMode screen;
	SDL_Texture *mountain, *city;

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


	mountain = load_texture_from_image("res/mountain.png", window, renderer);
	city = load_texture_from_image("res/city.png", window, renderer);


	for(int i = 1; i < 800; i++) {
		play_with_texture_2(mountain, window, renderer, i*0.1, i*0.1);
		play_with_texture_2(city, window, renderer, i*0.175, i*0.1);
		SDL_RenderPresent(renderer);             
		SDL_Delay(10);                         
		SDL_RenderClear(renderer);               // Effacer la fenêtre
	}

	SDL_Delay(200);
	SDL_RenderClear(renderer);

	IMG_Quit();

	end_sdl(1, "Normal ending", window, renderer);

	return 0;
}

