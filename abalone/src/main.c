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

#include "algos.h"
#include "init.h"
#include "graphics.h"
#include "utilities.h"

int main(void) {
	graphics_t *g = init_sdl();

	board_t *b = create_clean_board();
	// b = start_config(b);
	// b = start_config_2(b);
	cell_t **cell_tab=create_table(*b);

	/////////////////////////////
	// SDL MAIN LOOP FUNCTIONS //
	/////////////////////////////

	/*
	// First Event Loop
	int h;
	int w;
	int x=0;
	int y=0;
	int r1=0;
	int r2=0;
	int r=0;
	int mouse_state=0;
	SDL_GetWindowSize(g->window, &w, &h);

	// Rect creation
	SDL_Rect* text_box = crea_rect(w/3, h/4, w/3, h/4);
	SDL_Rect* button_1 = crea_rect(2*w/15, 5*h/9, h/4, h/4);
	SDL_Rect* button_2 = crea_rect(8*w/15, 5*h/9, h/4, h/4);

	SDL_Rect* text_box_2 = crea_rect(13*w/18, h/11, 2*w/9, 2*h/11);
	SDL_Rect* confirm = crea_rect(7*w/9, 4*h/11, w/9, h/11);
	SDL_Rect* button = crea_rect(13*w/18, 6*h/11, 2*w/9, 2*w/9);
	
	SDL_Texture*text=create_texture_for_text("choose your\nstarting line !", g->font, g->window, g->renderer);
	texturing(text,g->window, g->renderer);
	SDL_RenderPresent(g->renderer);
	SDL_Delay(200);

	SDL_bool program_on = SDL_TRUE;
	SDL_bool program_on_2 = SDL_FALSE;
	SDL_Event event;
	while (program_on) {
		// process event
		mouse_state=0;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				program_on = SDL_FALSE;
				break;
				case SDL_MOUSEBUTTONDOWN:
				x=event.button.x;
				y=event.button.y;
				mouse_state=1;
				break;
				case SDL_MOUSEBUTTONUP:
				mouse_state=2;
				x=event.button.x;
				y=event.button.y;
				break;
				default:
				break;
			}
		}

		// update
		if(mouse_state==1){
			if(is_in(button_1, x, y)){
				r1=255;
				r2=0;
			}
			else if (is_in(button_2, x, y)){
				r1=0;
				r2=255;
			}
		}
			
		if(mouse_state==2){
			r1=0;
			r2=0;
		}

		if(mouse_state==2){
			if(is_in(button_1, x, y)){
				b=start_config(b);
				program_on = SDL_FALSE;
				program_on_2 = SDL_TRUE;
			}
			else if (is_in(button_2, x, y)){
				b=start_config_2(b);
				program_on = SDL_FALSE;
				program_on_2 = SDL_TRUE;
			}
		}
		
		// render
		home_menu(g, text_box, button_1, button_2, text, r1, r2);
		SDL_Delay(1);
	}
	
	SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
	SDL_RenderClear(g->renderer);
			

	// Second Event Loop
	SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
	while (program_on_2) {
		
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				program_on_2 = SDL_FALSE;
				break;

				case SDL_MOUSEBUTTONDOWN:
				x=event.button.x;
				y=event.button.y;


				if(x>7*w/9 && 7*w/9+w/9){
					if(4*h/11 && 4*h/11+h/11){
						r=255;
					}
				}
				else{
					r=0;
				}
				break;
				case SDL_MOUSEBUTTONUP:
				x=event.button.x;
				y=event.button.y;
				break;

				default:
				break;
			}
			if(x>7*w/9 && 7*w/9+w/9){
				if(4*h/11 && 4*h/11+h/11){
					program_on_2 = SDL_FALSE;
					SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
					SDL_RenderClear(g->renderer);
				}
			}
			else{
				display_game(g, text_box_2, confirm, button, text, r, cell_tab);
				SDL_Delay(1);
			}
		}
	}
	*/


	// TEST FUNCTIONS

	// algos.c tests
	// /*
	
	display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);
	SDL_Delay(1000);

	play_t *p = malloc(sizeof(play_t));


	for(int i = 0; i < CELL_NUMBER; i++) {
		cell_tab[i]->id = i;
	}
	// */

	/*
	tree_t *t = malloc(sizeof(tree_t));
	t = gen_plays(b, 1, 1);
	while(t->next_tree != NULL) {
		if(validity_play(b, t->play, 1)) {
			print_play(b, t->play);

			apply_play(b, t->play);
			display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);
			SDL_Delay(500);
			undo_play(b, t->play);
			display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);
			SDL_Delay(500);
			b = create_clean_board();
			for(int i = 0; i < CELL_NUMBER; i++) {
				cell_tab[i]->state = EMPTY;
			}
			// b = start_config(b);
			b = start_config_2(b);
		}
		display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);
		SDL_Delay(100);
		t = t->next_tree;
	}
	*/

	int black_ids[9] = {0, 1, 3, 5, 6, 7, 12, 19, 28};
	int white_ids[14] = {9, 10, 13, 14, 16, 20, 21, 22, 23, 29, 30, 31, 37, 38};
	for (int k = 0; k < 9; k++) {
		cell_tab[black_ids[k]] -> state = BLACK;
	}
	for (int k = 0; k < 14; k++) {
		cell_tab[white_ids[k]] -> state = WHITE;
	}

	display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);
	SDL_Delay(1000);
	p = choose_play(b);

	b = apply_play(b, p);
	print_play(p);

	display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);
	SDL_Delay(2000);


	SDL_RenderClear(g->renderer);
	IMG_Quit();
	end_sdl(1, "Normal ending", g->window, g->renderer);

	return 0;

}
