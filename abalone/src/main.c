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

void score_update(board_t *b, cell_t **cell_tab){
	b->n_black=0;
	b->n_white=0;
	for(int i=0;i<61;i++){
		if(cell_tab[i]->state==BLACK){
			b->n_black+=1;
		}
		else if(cell_tab[i]->state==WHITE){
			b->n_white+=1;
		}
	}
}

int main(void) {

	// global initialisation
	graphics_t *g = init_sdl();
	board_t *b = create_clean_board();
	cell_t **cell_tab=create_table(*b);
	play_t *play=create_play();

///*
	/////////////////////////////
	// SDL MAIN LOOP FUNCTIONS //
	/////////////////////////////

	// initialisation of variables

	// mouse's position and state
	int x = 0;
	int y = 0;
	int mouse_state = 0;

	// colour modification (red part) to highlight clicked buttons
	int r1 = 0;
	int r2 = 0;
	int r = 0;

	// id of the cell pointed by the mouse
	int id_mouse_cell = 0;

	// the play is valid and the player play first
	bool is_play_selected_valid = 1;
	bool is_bot_turn = false;

	// Initialisation of the textures for the score display
	char *Text_Panel_Black= malloc(10*sizeof(char));
	char *Text_Panel_White= malloc(10*sizeof(char));

	sprintf(Text_Panel_Black, "Black : %d",b->n_black);
	sprintf(Text_Panel_White, "White : %d",b->n_white);

	SDL_Texture * texture_text_panel_black=create_texture_for_text(Text_Panel_Black, g->font, g->renderer, g->colours->black);
	SDL_Texture * texture_text_panel_white=create_texture_for_text(Text_Panel_White, g->font, g->renderer, g->colours->black);


	// Initialisation for the event loop
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

				default:
				break;
			}
		}

		// update

		// the bot turn to play
		if(is_bot_turn) {
			play = choose_play(b, cell_tab, 1);
			apply_play(b, play);
			printf("bot :\n");
			print_play(play);
			init_play(play);
			//play->cell_tab_length=0;
			is_bot_turn = false;
		}

		// the player turn to play
		else{

			// init the play as valid
			is_play_selected_valid = 1;

			// cell previously under the mouse's cursor back to being displayed in black
			if(id_mouse_cell>=0 && id_mouse_cell<61){
				if(cell_tab[id_mouse_cell]->selection==MOUSE){
					cell_tab[id_mouse_cell]->selection=UNSELECT;
				}
			}

			// get the id of the cell under the mouse's cursor
			id_mouse_cell = get_cell_id_from_mouse_position(g, x, y);

			// display the cell under the mouse's cursor in red
			if(mouse_state == 0){
				if(!(is_in(g->panel, x, y))){
					id_mouse_cell = get_cell_id_from_mouse_position(g, x, y);
					if(id_mouse_cell >= 0 && id_mouse_cell<61){
						if(cell_tab[id_mouse_cell]->selection == UNSELECT && cell_tab[id_mouse_cell]->state == BLACK){
							id_mouse_cell = get_cell_id_from_mouse_position(g, x, y);
							cell_tab[id_mouse_cell]->selection = MOUSE;
						}
					}
				}
			}

			// display the confirm button in pink when clicked on
			else if(mouse_state==1){
				if(is_in(g->confirm, x, y)){
					r=255;
				}
				else{
					r=0;
				}
			}

			// button clicked
			else if(mouse_state==2){
				r=0;

				// Confirm the play // maybe a problem, maybe not
				if(is_in(g->confirm, x, y)){
					if(play->cell_tab_length){

						// update and check for validity
						if(play->cell_direction==play->movement_direction){
							int input_length = play->cell_tab_length - 1;
							while(input_length < 5) {
								if (play->cell_tab[input_length]->neighbor[play->movement_direction] == NULL) {
									break;
								}
								if (play->cell_tab[input_length]->neighbor[play->movement_direction]->state == BLACK) {
									is_play_selected_valid = 0;
									break;
								}
								if (play->cell_tab[input_length]->neighbor[play->movement_direction]->state == EMPTY) {
									break;
								}
								if (play->cell_tab[input_length]->neighbor[play->movement_direction]->state == WHITE) {
									play->cell_tab[input_length + 1] = play->cell_tab[input_length]->neighbor[play->movement_direction];
									play->buffer[input_length + 1] = WHITE;
									input_length++;
									play->cell_tab_length++;
								}
								printf("input_length : %d\n", input_length);

							}
						}
						fill_play_buffer(play);
						if(play->cell_tab_length==1){
							play->cell_direction=play->movement_direction;
						}

						// valid play
						if (validity_play(play, 0) && is_play_selected_valid){
							b=apply_play(b, play);
							printf("player :\n");
							print_play(play);
							is_bot_turn = true;
						}

						// unvalid play
						else{
							is_play_selected_valid = 0;
						}

						// unselect the balls
						for(int k=0;k<play->cell_tab_length;k++){
							play->cell_tab[k]->selection=UNSELECT;
						}
						//play->cell_tab_length=0;
						init_play(play);
					}
				}

				// Choose in which direction to push the balls
				else if(is_in(g->commands_panel->button, x, y)){
					for(int i = 0; i < 6; i++){
						if (is_in(g->commands_panel->tab_dir[i], x, y)) {
							play->movement_direction = i;
							i = 6;
						}
					}
				}

				// Select/unselect the balls to move
				else if(id_mouse_cell>=0 && id_mouse_cell<61){
					if(cell_tab[id_mouse_cell]->state==BLACK){
						// unselect
						if(cell_tab[id_mouse_cell]->selection==SELECT && (cell_tab[id_mouse_cell]==play->cell_tab[play->cell_tab_length-1])){
							cell_tab[id_mouse_cell]->selection=UNSELECT;
							play->cell_tab[play->cell_tab_length] = NULL;
							play->cell_tab_length--;
						}
						// select
						else{
							play->cell_tab[play->cell_tab_length] = cell_tab[id_mouse_cell];
							play->cell_tab_length++;

							play->buffer[play->cell_tab_length] = cell_tab[id_mouse_cell]->state;

							cell_tab[id_mouse_cell]->selection=SELECT;
							if (play->cell_tab_length == 2){
								for(int k = 0; k < 6; k++){
									if(play->cell_tab[play->cell_tab_length-2]->neighbor[k] == cell_tab[id_mouse_cell]){
										play->cell_direction = k;
										k = 6;
									}
								}
							}
						}
					}
				}
			}

		}

		// update score
		score_update(b, cell_tab);

		// update textures for the score display
		sprintf(Text_Panel_Black, "Black : %d",b->n_black);
		sprintf(Text_Panel_White, "White : %d",b->n_white);
		texture_text_panel_black=create_texture_for_text(Text_Panel_Black, g->font, g->renderer, g->colours->yellow);
		texture_text_panel_white=create_texture_for_text(Text_Panel_White, g->font, g->renderer, g->colours->yellow);
		
		// render
		display_game(g, texture_text_panel_black, texture_text_panel_white, r, cell_tab, play->movement_direction, is_play_selected_valid);
		SDL_Delay(1);
	}

//*/

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


	///*
	SDL_RenderClear(g->renderer);

	IMG_Quit();

	end_sdl(1, "Normal ending", g->window, g->renderer);
	//*/

	free_board_and_cell_tab(b, cell_tab);

	return 0;

}
