/* name : main.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 18-06-24
 */

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_render.h>
#include<SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>

#include "SDL2/SDL_video.h"
#include "algos.h"
#include "init.h"
#include "graphics.h"
#include "utilities.h"

int main(void) {
	graphics_t *g = init_sdl();

	board_t *b = create_clean_board();
	// b = start_config_2(b);
	cell_t **cell_tab=create_table(*b);


	// TEST FUNCTIONS

	// /*
	// display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);
	// SDL_Delay(1000);

	play_t *p = malloc(sizeof(play_t));

	for(int i = 0; i < CELL_NUMBER; i++) {
		cell_tab[i]->id = i;
	}
	// */

	/*
	int black_ids[9] = {0, 1, 3, 5, 6, 7, 12, 19, 28};
	int white_ids[14] = {9, 10, 13, 14, 16, 20, 21, 22, 23, 29, 30, 31, 37, 38};
	for (int k = 0; k < 9; k++) {
		cell_tab[black_ids[k]] -> state = BLACK;
	}
	for (int k = 0; k < 14; k++) {
		cell_tab[white_ids[k]] -> state = WHITE;
	}
	b->n_white = 14;
	b->n_black = 9;
	*/

	/*
	tree_t *t = malloc(sizeof(tree_t));
	t = gen_plays(b, 1, 1);
	while(t->next_tree != NULL) {
		if(validity_play(t->play, 1)) {
			print_play(t->play);

			apply_play(b, t->play);
			display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);
			SDL_Delay(500);
			undo_play(b, t->play);
			display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);
			SDL_Delay(500);
			// b = create_clean_board();
			// for(int i = 0; i < CELL_NUMBER; i++) {
				// cell_tab[i]->state = EMPTY;
			// }
			// b = start_config(b);
			// b = start_config_2(b);
		}
		display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);
		SDL_Delay(100);
		t = t->next_tree;
	}
	*/

	// cell_tab[4]->state = WHITE;
	cell_tab[30]->state = BLACK;
	// cell_tab[30]->state = WHITE;
	display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);
	SDL_Delay(1000);

	p = choose_play(b, cell_tab, 0);
	b = apply_play(b, p);

	// /*
	while(true) {
		printf("white : \n");
		p = choose_play(b, cell_tab, 1);
		b = apply_play(b, p);
		display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);
		SDL_Delay(50);

		printf("black : \n");
		p = choose_play(b, cell_tab, 0);
		b = apply_play(b, p);
		display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);
		SDL_Delay(50);
	}
	// */

	display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);
	SDL_Delay(1000);

	/*
	SDL_bool program_on = SDL_TRUE, event_utile = SDL_FALSE;

	SDL_Event event;

	int x, y, i, id;
	bool is_empty = true;
	i = 0;

	while(program_on) {
		event_utile = SDL_FALSE;
		while(!event_utile && SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					program_on = SDL_FALSE;
					event_utile = SDL_TRUE;
				break;
				case SDL_MOUSEBUTTONDOWN:
					x = event.button.x;
					y = event.button.y;
					id = get_cell_id_from_mouse_position(g, x, y);
					p->cell_tab[i] = cell_tab[id];
					p->buffer[i] = cell_tab[id]->state;
					i++;
					printf("id : %d, x = %d, y = %d\n", get_cell_id_from_mouse_position(g, x, y), x, y);
				break;
				default:
				break;
			}
		}
		if(is_empty) {
			printf("Enter direction of cells : ");
			scanf("%d", &p->cell_direction);
			printf("\n");
			printf("Enter number of cells : ");
			scanf("%d", &p->cell_tab_length);
			printf("\n");
			printf("Enter direction of movement : ");
			scanf("%d", &p->movement_direction);
			printf("\n");
			is_empty = false;
		}

		if(i == p->cell_tab_length) {
			printf("%p\n", p);
			print_play(p);
			b = apply_play(b, p);
			is_empty = true;
			i = 0;
			display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);

			p = choose_play(b, cell_tab, 1);
			b = apply_play(b, p);
		}

		display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);


		SDL_Delay(500);
	}
	*/


	SDL_RenderClear(g->renderer);
	IMG_Quit();
	end_sdl(1, "Normal ending", g->window, g->renderer);

	return 0;

}
