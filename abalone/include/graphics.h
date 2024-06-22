/* name : graphics.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 18-06-24
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include "init.h"

#ifndef graphics_h 
#define graphics_h

/* Struct definitions */

/// contains all the graphics elements
typedef struct graphics {
	SDL_Window *window;
	SDL_Rect *window_dimensions;
	SDL_Renderer *renderer;
	SDL_Texture *board;
	SDL_Texture *white;
	SDL_Texture *black;
	SDL_Texture *red;
	SDL_Texture *config_1;
	SDL_Texture *config_2;
	SDL_Texture *commands;
	SDL_Texture *background;
	TTF_Font * font;
	struct commands_panel *commands_panel;
	SDL_Rect *panel;
	SDL_Rect *text_box;
	SDL_Rect *text_box_black;
	SDL_Rect *text_box_white;
	SDL_Rect *confirm;
	SDL_Texture *text_confirm;
	SDL_Texture *error_text;
	SDL_Texture *error_text_2;
	struct home_menu *home_menu;
	struct colours *colours;
} graphics_t;

typedef struct colours {
	SDL_Color yellow;
	SDL_Color black;
	SDL_Color dark_red;
} colours_t;

/// coordinates of a point in space
typedef struct coordinates {
	int x;
	int y;
} coordinates_t;

/// contains an SDL button a an array of SDL rectangles to handle the command panel
typedef struct commands_panel {
	SDL_Rect *button;
	SDL_Rect *tab_dir[6];
} commands_panel_t;


typedef struct home_menu {
	SDL_Texture *fond;
	SDL_Rect *text_box;
	SDL_Rect *button_1;
	SDL_Rect *button_2;
	SDL_Texture *text_home_menu;
} home_menu_t;

/* Functions definitions */

/// ends the SDL elements and handles errors
void end_sdl(char ok, char const *msg, SDL_Window *window, SDL_Renderer *renderer);

/// initialises the sdl components and returns them in a struct
graphics_t *init_sdl();

colours_t *init_colours();

/// initialises the commands_panel structure
commands_panel_t *init_commands_panel(int w, int h);

home_menu_t *init_home_menu(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font, int w, int h, colours_t *colours);

/// loads a texture from an image
SDL_Texture* load_texture_from_image(char  *  file_image_name, SDL_Window *window, SDL_Renderer *renderer );

/// creates a texture for a text
SDL_Texture* create_texture_for_text(char  *  text, TTF_Font * font, SDL_Renderer *renderer, SDL_Color colour);

/// create a pointer to a rectangle
SDL_Rect* crea_rect(int x, int y, int width, int height);

/// create a pointer to a rectangle inside another rectangle
SDL_Rect* crea_rect_in_rect(SDL_Rect *button, float i, float j, float k, float l);

SDL_Rect* crea_rect_in_rect_2(SDL_Rect *button, float i, float j, float k, float l);

/// displays the textures on the renderer
void texturing(SDL_Texture* my_texture, SDL_Window* window, SDL_Renderer* renderer);

/// displays a cell on the board given an id
void display_cell(SDL_Texture *texture, SDL_Window *window, SDL_Renderer *renderer, int id);

/// returns the id of the cell clicked on with the mouse
int get_cell_id_from_mouse_position(graphics_t *g, int x, int y);

/// displays current state of the board
void display_board(graphics_t *g, cell_t **cell_tab);

/// checks if a position (x, y) is in a rectangle button
int is_in (SDL_Rect* button,int x,int y);

/// shows the home menu
void home_menu(graphics_t* g, int r1,int r2);

/// displays the user interface during the game
void display_game(graphics_t *g,SDL_Texture * text_panel_black, SDL_Texture * text_panel_white, int r, cell_t **cell_tab, int direction_state, int is_play_selected_valid);

#endif

