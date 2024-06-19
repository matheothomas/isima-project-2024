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
						   SDL_WINDOWPOS_CENTERED, screen.h * 0.9 * 1.5,
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


	// TEST FUNCTIONS
	
	graphics_t *g = malloc(sizeof(graphics_t));
	g->window = window;
	g->renderer = renderer;
	g->board = board;
	g->white = white;
	g->black = black;

	board_t * b = create_clean_board();
	cell_t **cell_tab = create_table(*b);
	b = start_config(b);

	// algos.c tests
	
	display_board(board, white, black, window, renderer, cell_tab);
	SDL_Delay(1000);

	play_t *p = malloc(sizeof(play_t));

	tree_t *t = malloc(sizeof(tree_t));

	for(int i = 0; i < CELL_NUMBER; i++) {
		cell_tab[i]->id = i;
	}

	/*
	t = gen_plays(b, 1, 1);
	while(t->next_tree != NULL) {
		if(validity_play(b, t->play, 1)) {

			printf("ids : ");
			for(int i = 0; i < t->play->cell_tab_length; i++) {
				printf("%d ", t->play->cell_tab[i]->id);
			}
			printf("\n buffer : ");
			for(int i = 0; i < t->play->cell_tab_length; i++) {
				printf("%d ", t->play->buffer[i]);
			}
			printf("\ncell_tab_length : %d\n movement_direction : %d\n cell_direction %d\n", t->play->cell_tab_length, t->play->movement_direction, t->play->cell_direction);
			printf("validity : %d\n", validity_play(b, t->play, 1));

			apply_play(b, t->play);
			display_board(board, white, black, window, renderer, cell_tab);
			SDL_Delay(500);
			undo_play(b, t->play);
			display_board(board, white, black, window, renderer, cell_tab);
			SDL_Delay(500);
			// b = create_clean_board();
			for(int i = 0; i < CELL_NUMBER; i++) {
				cell_tab[i]->state = EMPTY;
			}
			b = start_config(b);
		}
		display_board(board, white, black, window, renderer, cell_tab);
		SDL_Delay(500);
		t = t->next_tree;
	}
	*/

	cell_tab[16]->state = BLACK;
	display_board(board, white, black, window, renderer, cell_tab);
	SDL_Delay(2000);

	p = choose_play(b);

	b = apply_play(b, p);

	display_board(board, white, black, window, renderer, cell_tab);
	SDL_Delay(4000);





	SDL_RenderClear(renderer);

	IMG_Quit();

	end_sdl(1, "Normal ending", window, renderer);

	return 0;

}
