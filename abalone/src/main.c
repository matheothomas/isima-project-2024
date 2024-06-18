/* name : main.c
 * author : matheo thomas, domitille vale, eloi petit
 * date : 18-06-24
 */

#include "SDL2/SDL_render.h"
#include "SDL2/SDL_timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/graphics.h"
#include "../include/algos.h"
// #include "../include/init.h"



int main(void){
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_DisplayMode screen;
	SDL_Texture *board;

	// SDL INITIALISATION
	if (SDL_Init(SDL_INIT_VIDEO) != 0) end_sdl(0, "ERROR SDL INIT", window, renderer);

	SDL_GetCurrentDisplayMode(0, &screen);

	// Window creation
	window = SDL_CreateWindow("Premier dessin",
						   SDL_WINDOWPOS_CENTERED,
						   SDL_WINDOWPOS_CENTERED, screen.w * 0.76,
						   screen.h * 0.76,
						   SDL_WINDOW_OPENGL);
	if (window == NULL) end_sdl(0, "ERROR WINDOW CREATION", window, renderer);

	// Renderer creation
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) end_sdl(0, "ERROR RENDERER CREATION", window, renderer);

	// Textures creation
	board = load_texture_from_image("res/board.png", window, renderer);
	texturing(board, window, renderer);

	SDL_RenderPresent(renderer);

	SDL_Delay(2000);

	SDL_RenderClear(renderer);

	IMG_Quit();

	end_sdl(1, "Normal ending", window, renderer);





	return 0;
}

