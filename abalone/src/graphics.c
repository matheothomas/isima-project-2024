/* name : graphics.c
 * author : matheo thomas
 * date : 18-06-24
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

void texturing(SDL_Texture* texture, SDL_Window* window, SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect source = {0}, window_dimensions = {0}, destination = {0};

	SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h);
	SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);

	float zoom = 0.9;
	destination.w = window_dimensions.w * zoom;						// the destination is a source's zoom
	destination.h = window_dimensions.h * zoom;
	destination.x = (window_dimensions.w - destination.w) / 2;
	destination.y = (window_dimensions.h - destination.h) / 2;


	// SDL_RenderCopyEx(renderer, my_texture, &source, &destination, j*0.1, &point, SDL_FLIP_NONE);
	SDL_RenderCopy(renderer, texture, &source, &destination);
}

void display_cell(SDL_Texture *texture, SDL_Window *window, SDL_Renderer *renderer, int id) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect source = {0}, window_dimensions = {0}, destination = {0};
	int i, j;

	SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h);
	SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);

	float zoom = 9;
	destination.w = window_dimensions.w / zoom;
	destination.h = window_dimensions.h / zoom;
	
	if(id < 5) {
		i = 1;
		j = id;
	} else if (id < 11) {
		i = 2;
		j = id - 5;
	} else if (id < 18) {
		i = 3;
		j = id - 11;
	} else if (id < 26) {
		i = 4;
		j = id - 18;
	} else if (id < 35) {
		i = 5;
		j = id - 26;
	} else if (id < 43) {
		i = 6;
		j = id - 35;
	} else if (id < 50) {
		i = 7;
		j = id - 43;
	} else if (id < 56) {
		i = 8;
		j = id - 50;
	} else if (id < 61) {
		i = 9;
		j = id - 56;
	}

	destination.x = destination.w * i;
	destination.y = destination.h * j;


	// SDL_RenderCopyEx(renderer, my_texture, &source, &destination, j*0.1, &point, SDL_FLIP_NONE);
	SDL_RenderCopy(renderer, texture, &source, &destination);
	
}
