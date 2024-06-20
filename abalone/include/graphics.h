/* name : graphics.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 18-06-24
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_ttf.h>

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
	SDL_Texture *red;
	SDL_Texture *config_1;
	SDL_Texture *config_2;
	SDL_Texture *commands;
	TTF_Font * font;
	struct commands_panel* commands_panel;
} graphics_t;

typedef struct coordinates {
	int x;
	int y;
} coordinates_t;

typedef struct commands_panel {
	SDL_Rect* button;
	SDL_Rect* dir_0;
	SDL_Rect* dir_1;
	SDL_Rect* dir_2;
	SDL_Rect* dir_3;
	SDL_Rect* dir_4;
	SDL_Rect* dir_5;
} commands_panel_t;


/* Functions definitions */

/// ends the SDL elements and handles errors
void end_sdl(char ok, char const *msg, SDL_Window *window, SDL_Renderer *renderer);

/// initialises the sdl components and returns them in a struct
graphics_t *init_sdl();

/// initialises the commands_panel structure
commands_panel_t *init_commands_panel(SDL_Window *window);

/// loads a texture from an image
SDL_Texture* load_texture_from_image(char  *  file_image_name, SDL_Window *window, SDL_Renderer *renderer );

/// creates a texture for a text
SDL_Texture* create_texture_for_text(char  *  text, TTF_Font * font, SDL_Window *window, SDL_Renderer *renderer );

/// create a pointer to a rect
SDL_Rect* crea_rect(int x, int y, int width, int height);

/// create a pointer to a rect inside another rect
SDL_Rect* crea_rect_in_rect(SDL_Rect *button, int i, int j);

/// displays the textures on the renderer
void texturing(SDL_Texture* my_texture, SDL_Window* window, SDL_Renderer* renderer);

/// displays a cell on the board given an id
void display_cell(SDL_Texture *texture, SDL_Window *window, SDL_Renderer *renderer, int id);

///
int get_cell_id_from_mouse_position(graphics_t *g, int x, int y);

/// displays current state of the board
void display_board(graphics_t *g, cell_t **cell_tab);

/// check if a position (x,y) is in a rect button
int is_in (SDL_Rect* button,int x,int y);

/// shows the home menu
void home_menu(graphics_t *g, SDL_Rect* text_box,SDL_Rect* button_1,SDL_Rect* button_2,SDL_Texture * text,int r1,int r2);

/// display user interface during the game
void display_game(graphics_t *g,SDL_Rect* text_box,SDL_Rect* confirm,SDL_Texture * text, int r, cell_t **cell_tab);

#endif

