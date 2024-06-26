/* name : graphics.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
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

	SDL_Rect *board_player;
	SDL_Rect *board_bot;
	SDL_Rect *panel;

	SDL_Rect *bot_rect;
	SDL_Rect *score_bot;
	SDL_Rect *player_rect;
	SDL_Rect *score_player;
	SDL_Rect *left_arrow;
	SDL_Rect *right_arrow;
	SDL_Rect *first_tile;
	SDL_Rect *second_tile;
	SDL_Rect *deck;
	SDL_Rect *tiles_in_deck;

	SDL_Texture *bot_text;
	SDL_Texture *player_text;

	SDL_Texture *left_arrow_text;
	SDL_Texture *right_arrow_text;

    struct colours *colours;
	SDL_Texture **type_texture;
    SDL_Texture *background;
    TTF_Font * font;
	float offset_x;
	float offset_y;
}graphics_t;

typedef struct coordinates {
	int x;
	int y;
} coordinates_t;

typedef struct colours {
	SDL_Color yellow;
	SDL_Color black;
	SDL_Color dark_red;
	SDL_Color white;
} colours_t;

typedef struct type_texture{
	SDL_Texture *empty;
	SDL_Texture *blue;
	SDL_Texture *red;
	SDL_Texture *yellow;
	SDL_Texture *purple;
	SDL_Texture *green;
	SDL_Texture *grey;
	SDL_Texture *blue_place;
	SDL_Texture *yellow_place;
	SDL_Texture *red_place;
	SDL_Texture *purple_place;
	SDL_Texture *green_place;
}type_texture_t;



/* Functions definitions */

/// ends the SDL elements and handles errors
void end_sdl(char ok, char const *msg, SDL_Window *window, SDL_Renderer *renderer);

/// initialises the sdl components and returns them in a struct
graphics_t *init_sdl();

colours_t *init_colours();

type_texture_t *init_type_texture( SDL_Window *window, SDL_Renderer *renderer);

/// loads a texture from an image
SDL_Texture* load_texture_from_image(char  *  file_image_name, SDL_Window *window, SDL_Renderer *renderer );

/// creates a texture for a text
SDL_Texture* create_texture_for_text(char  *  text, TTF_Font * font, SDL_Renderer *renderer, SDL_Color colour);

/// create a pointer to a rectangle
SDL_Rect* crea_rect(int x, int y, int width, int height);

/// create a pointer to a rectangle inside another rectangle
SDL_Rect* crea_rect_in_rect(SDL_Rect *button, float i, float j, float k, float l);

/// checks if a position (x, y) is in a rectangle button
int is_in (SDL_Rect* button,int x,int y);

/// checks if a position (x, y) is in the hexagon corresponding to a rectangle "dest"
int is_in_hexa (SDL_Rect dest, int x, int y, int offset_x, int offset_y);

void display_cell(SDL_Texture *texture, graphics_t *graphics, int id, int altitude, int decal);

int get_cell_id_from_mouse_position(graphics_t *graphics, int x, int y, int decal);

void display_board(graphics_t *g, game_t *game, board_t * board, int decal, int x, int y);

void display_tile_in_rect(SDL_Rect *rect, tile_t *tile, graphics_t *graphics);

void display_mouse_cells(tile_t *tile, cell_t *cell, graphics_t *graphics);

void update_tile_position(tile_t *tile, cell_t *cell, game_t *game);

void display_game(graphics_t* g,  game_t *game, int x, int y);


#endif

