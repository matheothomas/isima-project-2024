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
	

	board_t * b = create_clean_board();
	cell_t **cell_tab = create_table(*b);
	b = start_config(b);

	play_t *p = malloc(sizeof(play_t));
	p->cell_tab[0] = cell_tab[21];
	// p->cell_tab[1] = cell_tab[14];
	p->cell_tab_length = 1;
	p->cell_direction = 0;
	p->movement_direction = 4;
	b = apply_play(b, p, 0);
	// p = choose_play(b);


	for(int i = 0; i < 61; i++) {
		if(cell_tab[i]->state == WHITE) {
			display_cell(white, window, renderer, i);
		} else if (cell_tab[i]->state == BLACK) {
			display_cell(black, window, renderer, i);
		}
	}

	SDL_RenderPresent(renderer);

	SDL_Delay(4000);

	SDL_RenderClear(renderer);

	IMG_Quit();

	end_sdl(1, "Normal ending", window, renderer);


	// TEST FUNCTIONS

	board_t * b = create_clean_board();
	cell_t **table=create_table(*b);
	
	b=start_config(b);

	for(int i=0;i<61;i++){
		printf("%d : %d \n",i,table[i]->state);
	}

    /*
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
	*/

	// algos.c tests


	return 0;

}
