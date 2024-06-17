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

	SDL_GetWindowSize(window, 
				   &window_dimensions.w,
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


void play_with_texture_4(SDL_Texture* my_texture, SDL_Window* window, SDL_Renderer* renderer, SDL_Texture *mountain, SDL_Texture *city) {
	SDL_Rect 
	source = {0},                    // Rectangle définissant la zone totale de la planche
	window_dimensions = {0},         // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
	destination = {0},               // Rectangle définissant où la zone_source doit être déposée dans le renderer
	state = {0};                     // Rectangle de la vignette en cours dans la planche 

	SDL_GetWindowSize(window,              // Récupération des dimensions de la fenêtre
				   &window_dimensions.w,
				   &window_dimensions.h);
	SDL_QueryTexture(my_texture,           // Récupération des dimensions de l'image
				  NULL, NULL,
				  &source.w, &source.h);

	/* Mais pourquoi prendre la totalité de l'image, on peut n'en afficher qu'un morceau, et changer de morceau :-) */

	int nb_images = 12;                     // Il y a 8 vignette dans la ligne de l'image qui nous intéresse
	float zoom = 2;                        // zoom, car ces images sont un peu petites
	int offset_x = source.w / nb_images,   // La largeur d'une vignette de l'image, marche car la planche est bien réglée
	offset_y = source.h / 8;           // La hauteur d'une vignette de l'image, marche car la planche est bien réglée


	state.x = 0 ;                          // La première vignette est en début de ligne
	state.y = 6 * offset_y;                // On s'intéresse à la 4ème ligne, le bonhomme qui court
	state.w = offset_x;                    // Largeur de la vignette
	state.h = offset_y;                    // Hauteur de la vignette

	destination.w = offset_x * zoom+350;       // Largeur du sprite à l'écran
	destination.h = offset_y * zoom+350;       // Hauteur du sprite à l'écran

	destination.y =                        // La course se fait en milieu d'écran (en vertical)
		(window_dimensions.h - destination.h) /2;

	int speed = 9;
	for (int x = 0; x < window_dimensions.w - destination.w; x += speed) {
		destination.x = x;                   // Position en x pour l'affichage du sprite
		state.x += offset_x;                 // On passe à la vignette suivante dans l'image
		state.x %= source.w;                 // La vignette qui suit celle de fin de ligne est
		// celle de début de ligne

		SDL_RenderClear(renderer);           // Effacer l'image précédente avant de dessiner la nouvelle
		play_with_texture_2(mountain, window, renderer, x*0.1, x*0.1);
		SDL_RenderCopy(renderer, my_texture, // Préparation de l'affichage
				 &state,
				 &destination);  
		play_with_texture_2(city, window, renderer, x*0.175, x*0.1);
		SDL_RenderPresent(renderer);         // Affichage
		SDL_Delay(70);                       // Pause en ms
	}
	SDL_RenderClear(renderer);             // Effacer la fenêtre avant de rendre la main
}



int main(void){
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_DisplayMode screen;
	SDL_Texture *mountain, *city, *bird;

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
	bird = load_texture_from_image("res/tile.png", window, renderer);

	play_with_texture_4(bird, window, renderer, mountain, city);



	// SDL_Delay(2000);

	// for(int i = 1; i < 800; i++) {
		// play_with_texture_2(mountain, window, renderer, i*0.1, i*0.1);
		// play_with_texture_2(city, window, renderer, i*0.175, i*0.1);
		// SDL_RenderPresent(renderer);             
		// SDL_Delay(10);                         
		// SDL_RenderClear(renderer);               // Effacer la fenêtre
	// }

	SDL_Delay(200);
	SDL_RenderClear(renderer);

	IMG_Quit();

	end_sdl(1, "Normal ending", window, renderer);

	return 0;
}

