/* name : main.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 18-06-24
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

#include "algos.h"
#include "graphics.h"
#include "init.h"
#include "utilities.h"

int main(void) {
	graphics_t *g = init_sdl();

	board_t *b = create_clean_board();
	cell_t **cell_tab=create_table(*b);

	play_t *play=malloc(sizeof(play_t));
	play->cell_direction=0;
	play->cell_tab_length=0;
	play->movement_direction=0;

	int h;
	int w;
	int x = 0;
	int y = 0;
	int r1 = 0;
	int r2 = 0;
	int r = 0;
	int mouse_state = 0;
	int id_mouse_cell;
	int nb_selected_cells = 0;
	cell_t *cur_cell;

	/////////////////////////////
	// SDL MAIN LOOP FUNCTIONS //
	/////////////////////////////

	SDL_GetWindowSize(g->window, &w, &h);

	// Rect and texture creation for the second event loop
	SDL_Rect* text_box_2 = crea_rect(13*w/18, h/11, 2*w/9, 2*h/11);
	SDL_Rect* confirm = crea_rect(7*w/9, 4*h/11, w/9, h/11);


	SDL_bool program_on = SDL_TRUE;
	SDL_bool program_on_2 = SDL_FALSE;
	SDL_Event event;

	// First Event Loop
	while (program_on) {

		// process event
		mouse_state=0;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {

				case SDL_QUIT:
					program_on = SDL_FALSE;
				break;

				case SDL_MOUSEMOTION:
					x=event.button.x;
					y=event.button.y;
				break;

				case SDL_MOUSEBUTTONDOWN:
					x=event.button.x;
					y=event.button.y;
					mouse_state=1;
				break;

				case SDL_MOUSEBUTTONUP:
					x=event.button.x;
					y=event.button.y;
					mouse_state=2;
				break;

				default:
				break;
			}
		}

		// update
		if(mouse_state==1){
			if(is_in(g->home_menu->button_1, x, y)){
				r1=255;
				r2=0;
			}
			else if (is_in(g->home_menu->button_2, x, y)){
				r1=0;
				r2=255;
			}
			else{
				r1=0;
				r2=0;
			}
		}

		if(mouse_state==2){
			r1=0;
			r2=0;
			if(is_in(g->home_menu->button_1, x, y)){
				b=start_config(b);
				program_on = SDL_FALSE;
				program_on_2 = SDL_TRUE;
			}
			else if (is_in(g->home_menu->button_2, x, y)){
				b=start_config_2(b);
				program_on = SDL_FALSE;
				program_on_2 = SDL_TRUE;
			}
		}

		// render
		home_menu(g, r1, r2);
		SDL_Delay(1);
	}


	// Second Event Loop

	// TO DO
	// let the bot play
	// add the possibility to unselect a ball
	// check if mouse position on the board
	// make the selection prettier

	// Texts
	
	bool is_bot_turn = false;

	SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
	while (program_on_2) {
		// process event
		mouse_state=0;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {

				case SDL_QUIT:
					program_on_2 = SDL_FALSE;
				break;

				case SDL_MOUSEMOTION:
					x=event.button.x;
					y=event.button.y;
				break;

				case SDL_MOUSEBUTTONDOWN:
					x=event.button.x;
					y=event.button.y;
					mouse_state=1;
				break;

				case SDL_MOUSEBUTTONUP:
					x=event.button.x;
					y=event.button.y;
					mouse_state=2;
				break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {

						case SDLK_0:
						case SDLK_KP_0:
							play->movement_direction=0;
						break;

						case SDLK_1:
						case SDLK_KP_1:
							play->movement_direction=1;
						break;

						case SDLK_2:
						case SDLK_KP_2:
							play->movement_direction=2;
						break;

						case SDLK_3:
						case SDLK_KP_3:
							play->movement_direction=3;
						break;

						case SDLK_4:
						case SDLK_KP_4:
							play->movement_direction=4;
						break;

						case SDLK_5:
						case SDLK_KP_5:
							play->movement_direction=5;
						break;

						default:
						break;
				}

				default:
				break;
			}
		}

		// update
		if(mouse_state==0){
			//id_mouse_cell=get_cell_id_from_mouse_position(g, x, y);
			//cell_tab[id_mouse_cell]->selection=MOUSE;
		}

		else if(mouse_state==1){
			if(is_in(confirm, x, y)){
				r=255;
			}
			else{
				r=0;
			}
		}

		else if(mouse_state==2){
			r=0;

			// Confirm the play
			if(is_in(confirm, x, y)){
				cur_cell= play->cell_tab[play->cell_tab_length-1];
				if(play->cell_direction==play->movement_direction){
					while (cur_cell && cur_cell->state && play->cell_tab_length<6) {
						cur_cell=cur_cell->neighbor[play->cell_direction];
						if(cur_cell || cur_cell->state || play->cell_tab_length<6){
							nb_selected_cells++;
							play->cell_tab[nb_selected_cells-1] = cur_cell;
							play->cell_tab_length++;
							play->buffer[nb_selected_cells-1] = cur_cell->state;
						}
					}
				}
				//fill_play_buffer(play);
				//print_play(b, play);
				if (validity_play(play, 0)){
					b=apply_play(b, play);
					is_bot_turn = true;
				}
				else{
					printf("coup non valide, réinitialitation du coup\n");
				}
				for(int k=0;k<play->cell_tab_length;k++){
					play->cell_tab[k]->selection=UNSELECT;
				}
				nb_selected_cells=0;
				play->cell_tab_length=0;
			}

			// Choose in which direction to push the balls
			else if(is_in(g->commands_panel->button, x, y)){
				for(int i=0;i<6;i++){
					if (is_in(g->commands_panel->tab_dir[i], x, y)) {
						play->movement_direction=i;
						i=6;
					}
				}
			}

			// Select the balls to move
			else if(1){ // TO DO check if mouse position on the board
				id_mouse_cell=get_cell_id_from_mouse_position(g, x, y);
				if(cell_tab[id_mouse_cell]->state==BLACK){
					if(cell_tab[id_mouse_cell]->selection==SELECT){ // TO DO add the possibility to unselect a ball
						//cell_tab[id_mouse_cell]->selection=UNSELECT;
						//nb_selected_cells-=1;
						//printf("%d %d\n",nb_selected_cells, play->movement_direction);
					}
					else{
						play->cell_tab[nb_selected_cells] = cell_tab[id_mouse_cell];
						play->cell_tab_length++;
						play->buffer[nb_selected_cells] = cell_tab[id_mouse_cell]->state;
						cell_tab[id_mouse_cell]->selection=SELECT;
						nb_selected_cells++;
						if (nb_selected_cells==2){
							for(int k=0;k<6;k++){
								if(play->cell_tab[nb_selected_cells-2]->neighbor[k]==cell_tab[id_mouse_cell]){
									play->cell_direction=k;
									k=6;
								}
							}
						}
						printf("%d %d %d\n",nb_selected_cells, play->movement_direction, play->cell_direction);
					}
				}
			}
		}

		if(is_bot_turn) {
			play = choose_play(b, cell_tab, 1);
			apply_play(b, play);
			is_bot_turn = false;
		}
		// render
		display_game(g, text_box_2, confirm, g->home_menu->text_home_menu, r, cell_tab, play->movement_direction);
		/*
		if(cell_tab[id_mouse_cell]->selection==MOUSE){
			cell_tab[id_mouse_cell]->selection=UNSELECT;
		}
		*/
		SDL_Delay(1);
	}



	// TEST FUNCTIONS

	/*
	for(int i=0;i<61;i++){
		printf("%d : %d \n",i,table[i]->state);
	}

	b -> cell -> state = BLACK;
	b -> cell -> neighbor[0] -> state = BLACK;
	b -> cell -> neighbor[0] -> neighbor[0] -> state = WHITE;
	b -> cell -> neighbor[0] -> neighbor[0] -> neighbor[0] -> state = BLACK;

	play_t play;
	play.cell_tab[0] = b -> cell;
	play.cell_tab[1] = b -> cell -> neighbor[0];
	play.cell_tab[2] = b -> cell -> neighbor[0] -> neighbor[0];
	play.cell_tab[3] = b -> cell -> neighbor[0] -> neighbor[0] -> neighbor[0];
	play.cell_tab[4] = NULL;
	play.cell_direction = 0;
	play.movement_direction = 0;
	play.cell_tab_length = 4;
	bool player = 0;

	printf("Validity play result : %d\n", validity_play(b, &play, player));
	*/ 

	// algos.c tests
	/*

	display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);
	SDL_Delay(1000);

	play_t *p = malloc(sizeof(play_t));

	tree_t *t = malloc(sizeof(tree_t));

	for(int i = 0; i < CELL_NUMBER; i++) {
		cell_tab[i]->id = i;
	}
	*/

	/*
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
			for(int i = 0; i < CELL_NUMBER; i++) {
				cell_tab[i]->state = EMPTY;
			}
			b = start_config(b);
		}
		display_board(g->board, g->white, g->black, g->window, g->renderer, cell_tab);
		SDL_Delay(100);
		t = t->next_tree;
	}
	*/

	/*
	play_t *p = malloc(sizeof(play_t));

	// cell_tab[4]->state = WHITE;
	cell_tab[30]->state = BLACK;
	// cell_tab[30]->state = WHITE;
	display_board(g, cell_tab);
	SDL_Delay(1000);

	p = choose_play(b, cell_tab, 0);
	b = apply_play(b, p);

	while(true) {
		printf("white : \n");
		p = choose_play(b, cell_tab, 1);
		b = apply_play(b, p);
		display_board(g, cell_tab);
		SDL_Delay(50);

		printf("black : \n");
		p = choose_play(b, cell_tab, 0);
		b = apply_play(b, p);
		display_board(g, cell_tab);
		SDL_Delay(50);
	}

	display_board(g, cell_tab);
	SDL_Delay(4000);

	*/



	SDL_RenderClear(g->renderer);

	IMG_Quit();

	end_sdl(1, "Normal ending", g->window, g->renderer);

	return 0;

}
