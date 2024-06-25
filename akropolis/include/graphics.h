/* name : graphics.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include "init.h"

#ifndef graphics_h 
#define graphics_h

/* Struct definitions */
typedef struct graphics{
    SDL_Window *window;
	SDL_Renderer *renderer;
    SDL_Rect *window_dimensions;
    struct colours *colours;
	SDL_Texture *blue;
	SDL_Texture *red;
	SDL_Texture *yellow;
	SDL_Texture *purple;
	SDL_Texture *grey;
	SDL_Texture *green;
    SDL_Texture *background;
    TTF_Font * font;
}graphics_t;

typedef struct coordinates {
	int x;
	int y;
} coordinates_t;

typedef struct colours {
	SDL_Color yellow;
	SDL_Color black;
	SDL_Color dark_red;
} colours_t;




/* Functions definitions */

/// ends the SDL elements and handles errors
void end_sdl(char ok, char const *msg, SDL_Window *window, SDL_Renderer *renderer);

/// initialises the sdl components and returns them in a struct
graphics_t *init_sdl();

colours_t *init_colours();

/// loads a texture from an image
SDL_Texture* load_texture_from_image(char  *  file_image_name, SDL_Window *window, SDL_Renderer *renderer );

/// creates a texture for a text
SDL_Texture* create_texture_for_text(char  *  text, TTF_Font * font, SDL_Renderer *renderer, SDL_Color colour);

/// create a pointer to a rectangle
SDL_Rect* crea_rect(int x, int y, int width, int height);

/// create a pointer to a rectangle inside another rectangle
SDL_Rect* crea_rect_in_rect(SDL_Rect *button, float i, float j, float k, float l);


#endif

