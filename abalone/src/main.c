/* name : main.c
 * author : matheo thomas, domitille vale, eloi petit
 * date : 18-06-24
 */

#include "SDL2/SDL_timer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/algos.h"
#include "../include/init.h"
#include "../include/graphics.h"
#include "../include/utilities.h"
#include "SDL2/SDL_render.h"

int main(void) {
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_DisplayMode screen;
	SDL_Texture *board, *white, *black;

	// SDL INITIALISATION
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		end_sdl(0, "ERROR SDL INIT", window, renderer);

	SDL_GetCurrentDisplayMode(0, &screen);

	// Window creation
	window = SDL_CreateWindow("Premier dessin", SDL_WINDOWPOS_CENTERED,
						   SDL_WINDOWPOS_CENTERED, screen.h * 0.9,
						   screen.h * 0.9, SDL_WINDOW_OPENGL);
	if (window == NULL)
		end_sdl(0, "ERROR WINDOW CREATION", window, renderer);

	// Renderer creation
	renderer = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
		end_sdl(0, "ERROR RENDERER CREATION", window, renderer);

	// Textures creation
	board = load_texture_from_image("res/board.png", window, renderer);
	white = load_texture_from_image("res/white.png", window, renderer);
	black = load_texture_from_image("res/black.png", window, renderer);
	texturing(board, window, renderer);

	SDL_RenderPresent(renderer);

	SDL_Delay(1);

	SDL_RenderClear(renderer);

	IMG_Quit();

	end_sdl(1, "Normal ending", window, renderer);


	// TEST FUNCTIONS

	board_t * b = create_clean_board();

	b -> cell -> state = BLACK;
	b -> cell -> neighbourg[0] -> state = BLACK;
	b -> cell -> neighbourg[0] -> neighbourg[0] -> state = WHITE;
	b -> cell -> neighbourg[0] -> neighbourg[0] -> neighbourg[0] -> state = BLACK;

	play_t play;
	play.cell_tab[0] = b -> cell;
	play.cell_tab[1] = b -> cell -> neighbourg[0];
	play.cell_tab[2] = b -> cell -> neighbourg[0] -> neighbourg[0];
	play.cell_tab[3] = b -> cell -> neighbourg[0] -> neighbourg[0] -> neighbourg[0];
	play.cell_tab[4] = NULL;
	play.cell_direction = 0;
	play.movement_direction = 0;
	play.cell_tab_length = 4;
	bool player = 0;

	printf("Validity play result : %d\n", validity_play(b, &play, player));

	return 0;

}
