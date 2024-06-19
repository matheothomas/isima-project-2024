/* name : graphics.h
 * author : matheo thomas
 * date : 18-06-24
 */

#include <SDL2/SDL.h>
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
#include "init.h"

#ifndef graphics_h 
#define graphics_h

/* Struct definitions */
typedef struct graphics {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *board;
	SDL_Texture *white;
	SDL_Texture *black;
} graphics_t;


/* Functions definitions */

/// ends the SDL elements and handles errors
void end_sdl(char ok, char const *msg, SDL_Window *window, SDL_Renderer *renderer);

/// loads a texture from an image
SDL_Texture* load_texture_from_image(char  *  file_image_name, SDL_Window *window, SDL_Renderer *renderer );

/// displays the textures on the renderer
void texturing(SDL_Texture* my_texture, SDL_Window* window, SDL_Renderer* renderer);

/// displays a cell on the board given an id
void display_cell(SDL_Texture *texture, SDL_Window *window, SDL_Renderer *renderer, int id);

/// returns a cell id corresponding to the mouse position
int get_cell_position(int x, int y);

/// displays current state of the board
void display_board(SDL_Texture *board, SDL_Texture *white, SDL_Texture *black, SDL_Window *window, SDL_Renderer *renderer, cell_t **cell_tab);


#endif

