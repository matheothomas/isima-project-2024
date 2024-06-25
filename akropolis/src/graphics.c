/* name : graphics.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include "algos.h"
#include "graphics.h"
#include "init.h"


void end_sdl(char ok, char const *msg, SDL_Window *window, SDL_Renderer *renderer) {              // renderer to close
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

graphics_t *init_sdl() {
    TTF_Init();
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
    SDL_Rect *window_dimensions;
	colours_t *colours;
	SDL_Texture *background, *blue, *red, *yellow, *green, *purple, *grey;
	TTF_Font * font;

    SDL_DisplayMode screen;

    // SDL INITIALISATION
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		end_sdl(0, "ERROR SDL INIT", window, renderer);

	SDL_GetCurrentDisplayMode(0, &screen);

    // Window creation
	window = SDL_CreateWindow("Premier dessin", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 
                            screen.h * 0.866 * 1.5,
						    screen.h * 1, SDL_WINDOW_OPENGL);

	if (window == NULL)
		end_sdl(0, "ERROR WINDOW CREATION", window, renderer);

	// Font opening
	font=TTF_OpenFont("res/Unique.ttf", 72 );

	// Rects creation
	window_dimensions=crea_rect(0, 0, screen.h * 0.866 * 1.5, screen.h * 1);

    // Renderer creation
	renderer = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
		end_sdl(0, "ERROR RENDERER CREATION", window, renderer);

	colours=init_colours();

    // Textures creation
    blue= load_texture_from_image("res/blue.png", window, renderer);
    red= load_texture_from_image("res/red.png", window, renderer);
    yellow= load_texture_from_image("res/yellow.png", window, renderer);
    purple= load_texture_from_image("res/purple.png", window, renderer);
    green= load_texture_from_image("res/green.png", window, renderer);
    grey= load_texture_from_image("res/grey.png", window, renderer);
	background= load_texture_from_image("res/background.jpg", window, renderer);

    graphics_t *graphics = malloc(sizeof(graphics_t));

    graphics->window=window;
    graphics->renderer=renderer;
    graphics->window_dimensions=window_dimensions;
    graphics->blue=blue;
    graphics->red=red;
    graphics->yellow=yellow;
    graphics->purple=purple;
    graphics->green=green;
    graphics->grey=grey;
    graphics->background=background;
    graphics->colours=colours;
    graphics->font=font;

    return graphics;
}

colours_t *init_colours(){

	colours_t *colours=malloc(sizeof(colours_t));

	SDL_Color yellow = {255,250,0,255}; 
	SDL_Color black = {0,0,0,255};
	SDL_Color dark_red = {25,20,20,255};

	colours->yellow=yellow;
	colours->black=black;
	colours->dark_red=dark_red;

	return colours;
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

SDL_Texture* create_texture_for_text(char  *  text, TTF_Font * font, SDL_Renderer *renderer, SDL_Color colour){       
	SDL_Surface * my_text = TTF_RenderText_Blended(font, text, colour);
	SDL_Texture * my_texture = SDL_CreateTextureFromSurface(renderer, my_text);
	SDL_FreeSurface(my_text);
	return my_texture;
}

SDL_Rect* crea_rect(int x, int y, int width, int height){
	SDL_Rect* rect=(SDL_Rect*)malloc(sizeof(SDL_Rect));
	rect->x = x;
	rect->y = y;
	rect->w = width;
	rect->h = height;
	return rect;
}

SDL_Rect* crea_rect_in_rect(SDL_Rect *button, float i, float j, float k, float l){
	SDL_Rect *dir=crea_rect(button->x+((float)i*(button->w)), button->y+((float)j*(button->h)), k*(button->w), l*(button->h));
	return dir;
}