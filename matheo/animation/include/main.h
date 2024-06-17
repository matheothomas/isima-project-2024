/* name : main.h
 * author : matheo thomas
 * date : 17-06-24
 */

#include "SDL2/SDL_render.h"
#include <SDL2/SDL.h>

#ifndef main_h 
#define main_h



/* Functions definitions */

/// Ends the SDL elements (renderer, window...) and handles errors.
void end_sdl(char ok, char const *msg, SDL_Window *window, SDL_Renderer *renderer);

/// Loads an image in a texture 
SDL_Texture *load_texture_from_image( char *file_image_name, SDL_Window *window, SDL_Renderer *renderer);

/// Prints a texture on the renderer
void play_with_texture_2(SDL_Texture *my_texture, SDL_Window *window, SDL_Renderer *renderer, float i, int j);

#endif

