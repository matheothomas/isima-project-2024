/* name : main.c
 * author : matheo thomas
 * date : 17-06-24
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "../include/main.h"

void end_sdl(char ok,                               // normal end : ok = 0 ; abnormal ok = 1
			 char const *msg,                       // message to print
			 SDL_Window *window,                    // window to close
			 SDL_Renderer *renderer) {              // renderer to close
	char msg_formated[255];                                            
	int l;                                                     

	if (!ok) {										// printing what is wrong
		strncpy(msg_formated, msg, 250);                                         
		l = strlen(msg_formated);                                            
		strcpy(msg_formated + l, " : %s\n");                                     

		SDL_Log(msg_formated, SDL_GetError());                                   
	}                                                          

	if (renderer != NULL) {							// renderer's destruction if necessary
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
	if (window != NULL)   {                         // windows destruction if necessary
		SDL_DestroyWindow(window);
		window= NULL;
	}

	SDL_Quit();                                                    

	if (!ok) {
		exit(EXIT_FAILURE);                                                  
	}                                                          
}

SDL_Texture* load_texture_from_image(char  *  file_image_name, SDL_Window *window, SDL_Renderer *renderer ){
	SDL_Surface *my_image = NULL;
	SDL_Texture* my_texture = NULL;

	my_image = IMG_Load(file_image_name);
	if (my_image == NULL) end_sdl(0, "Chargement de l'image impossible", window, renderer);

	my_texture = SDL_CreateTextureFromSurface(renderer, my_image);
	SDL_FreeSurface(my_image);
	if (my_texture == NULL) end_sdl(0, "Echec de la transformation de la surface en texture", window, renderer);

	return my_texture;
}

void texturing(SDL_Texture* my_texture, SDL_Window* window, SDL_Renderer* renderer, float i, int j) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect 
	source = {0},
	window_dimensions = {0},
	destination = {0};

	SDL_Point point;

	SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h);
	SDL_QueryTexture(my_texture, NULL, NULL, &source.w, &source.h);

	float zoom = 1.2 + 0.01*i;
	destination.w = source.w * zoom;         // the destination is a source's zoom
	destination.h = source.h * zoom;
	destination.x = (window_dimensions.w - destination.w*0.8);
	destination.y = (window_dimensions.h - destination.h*0.9);

	point.x = source.w;
	point.y = source.h;

	SDL_RenderCopyEx(renderer, my_texture, &source, &destination, j*0.1, &point, SDL_FLIP_NONE);
}

void main_scene(SDL_Texture* my_texture, SDL_Window* window, SDL_Renderer* renderer, SDL_Texture *mountain, SDL_Texture *city) {
	SDL_Rect 
	source = {0},
	window_dimensions = {0},
	destination = {0},
	state = {0};

	SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h);
	SDL_QueryTexture(my_texture, NULL, NULL, &source.w, &source.h);				// image dimensions

	int nb_images = 12;                     // number of lines in the tile
	float zoom = 2;
	int offset_x = source.w / nb_images,
	offset_y = source.h / 8;				// number of columns in the tile


	state.x = 0 ;
	state.y = 6 * offset_y;                // 6th line to get the flying bird
	state.w = offset_x;
	state.h = offset_y;

	destination.w = offset_x * zoom+350;
	destination.h = offset_y * zoom+350;

	destination.y = (window_dimensions.h - destination.h) /2;

	int speed = 9;
	for (int x = 0; x < window_dimensions.w - destination.w; x += speed) {
		if(x > 500 && x < 650) {
			state.y = 4 * offset_y;
		} else {
			state.y = 6 * offset_y;
		}
		destination.x = x;
		state.x += offset_x;
		state.x %= source.w;

		SDL_RenderClear(renderer);
		texturing(mountain, window, renderer, x*0.1, x*0.1);
		SDL_RenderCopy(renderer, my_texture, &state, &destination);  
		texturing(city, window, renderer, x*0.175, x*0.1);
		SDL_RenderPresent(renderer);
		SDL_Delay(70);
	}
	
	SDL_RenderClear(renderer);
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

	// Textures creation
	mountain = load_texture_from_image("res/mountain.png", window, renderer);
	city = load_texture_from_image("res/city.png", window, renderer);
	bird = load_texture_from_image("res/tile.png", window, renderer);


	main_scene(bird, window, renderer, mountain, city);

	SDL_RenderClear(renderer);

	IMG_Quit();

	end_sdl(1, "Normal ending", window, renderer);

	return 0;
}

