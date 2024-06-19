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
#include "SDL2/SDL_render.h"

int main(void) {
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_DisplayMode screen;
	SDL_Texture *board;

	// SDL INITIALISATION
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		end_sdl(0, "ERROR SDL INIT", window, renderer);

	SDL_GetCurrentDisplayMode(0, &screen);

	// Window creation
	window = SDL_CreateWindow("Premier dessin", SDL_WINDOWPOS_CENTERED,
<<<<<<< HEAD
						   SDL_WINDOWPOS_CENTERED, screen.w * 0.76,
						   screen.h * 0.76, SDL_WINDOW_OPENGL);
=======
						   SDL_WINDOWPOS_CENTERED, screen.h * 0.9,
						   screen.h * 0.9, SDL_WINDOW_OPENGL);
>>>>>>> 4f1f29ecddbc92bf792ad74dfe8559e0f49a306b
	if (window == NULL)
		end_sdl(0, "ERROR WINDOW CREATION", window, renderer);

	// Renderer creation
	renderer = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
		end_sdl(0, "ERROR RENDERER CREATION", window, renderer);

	// Textures creation
	board = load_texture_from_image("res/board.png", window, renderer);
	texturing(board, window, renderer);

	SDL_RenderPresent(renderer);

<<<<<<< HEAD
	SDL_Delay(2);
=======
	SDL_Delay(3000);
>>>>>>> 4f1f29ecddbc92bf792ad74dfe8559e0f49a306b

	SDL_RenderClear(renderer);

	IMG_Quit();

	end_sdl(1, "Normal ending", window, renderer);

<<<<<<< HEAD
	board_t b = create_clean_board();
	
=======

	// TEST FUNCTIONS

	board_t b = create_clean_board();

>>>>>>> 4f1f29ecddbc92bf792ad74dfe8559e0f49a306b
	return 0;

}
