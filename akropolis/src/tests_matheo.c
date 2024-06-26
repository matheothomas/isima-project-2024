/* name : tests_matheo.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */


#include "hash_map.h"
#include <stdbool.h>
#define DEBUG

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
#include <time.h>

#include "init.h"
#include "utilities.h"
#include "algos.h"
#include "graphics.h"

#include "tests_matheo.h"


void test_matheo(){

	srand(time(0));

	graphics_t *g = init_sdl();

	board_t *player = create_board();
	tile_t *first_player = create_tile();
	fill_tile(first_player, BLUE_PLACE,  QUARRY_GRAY, QUARRY_GRAY);
	first_player->cell_tab[0] = player->cell;
	first_player->cell_tab[1] = player->cell->neighbour[0];
	first_player->cell_tab[2] = player->cell->neighbour[1];
	add_tile(first_player);

	board_t *bot=create_board();
	tile_t *first_bot=create_tile();
	fill_tile(first_bot, BLUE_PLACE,  QUARRY_GRAY, QUARRY_GRAY);
	first_bot->cell_tab[0] = bot->cell;
	first_bot->cell_tab[1] = bot->cell->neighbour[0];
	first_bot->cell_tab[2] = bot->cell->neighbour[1];

	deck_t *deck = create_deck();
	init_deck(deck);

	game_t *game = create_game();
	update_game(game, player, bot, deck->deck[0], deck->deck[1], deck);

	tile_t *tile = game->deck->deck[3];
	tile->cell_tab[0] = game->player->cell->neighbour[0]->neighbour[0];
	tile->cell_tab[1] = game->player->cell->neighbour[0]->neighbour[0]->neighbour[0];
	tile->cell_tab[2] = game->player->cell->neighbour[0]->neighbour[0]->neighbour[1];


	hash_t **h = create_hash_map();
	printf("%zu\n", hash_board(game->player));

	linked_plays_t *lp = gen_tiles_from_game(game, false);
	hash_map_add(h, game->player, lp);
	printf("%zu\n", hash_board(game->player));

	add_tile_to_board(game->player, tile);
	lp = gen_tiles_from_game(game, false);
	hash_map_add(h, game->player, lp);
	printf("%zu\n", hash_board(game->player));


	for (int i = 0; i <= HASHMAP_SIZE; i++) {
		hash_t *h_cell = h[i];
		while(h_cell != NULL) {
			printf("h[%d] : %zu\n", i, h_cell->hashed_board);
			h_cell = h_cell->next;
		}
	}

	game->deck->n = 2;

	lp = gen_tiles_from_game(game, false);
	play_t *play = get_random_tile(lp);
	add_tile_to_board(game->player, play->tile);
	printf("card_1 : %p card2 : %p n : %d\n", game->card_1, game->card_2, game->deck->n);
	update_deck(game, play->tile);
	printf("card_1 : %p card2 : %p n : %d\n", game->card_1, game->card_2, game->deck->n);
	undo_deck(game, play->tile);
	printf("card_1 : %p card2 : %p n : %d\n", game->card_1, game->card_2, game->deck->n);

	/*
	for(int i = 0; i < DECK_NUMBER - 2; i++) {
		display_game(g, game);
		SDL_Delay(200);

		lp = gen_tiles_from_game(game, false);
		play_t *play = get_random_tile(lp);
		add_tile_to_board(game->player, play->tile);
		update_deck(game, play->tile);

	}
	*/

	// remove_tile_from_board(game->player, tile);

	free_hash_map(h);


	// initialisation of variables

	// mouse's position and state
	int x = 0;
	int y = 0;
	int mouse_state = 0;
	int id_mouse_cell=-1;

	// Initialisation for the event loop
	SDL_bool program_on = SDL_TRUE;
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
					x = event.button.x;
					y = event.button.y;
				break;

				case SDL_MOUSEBUTTONDOWN:
					x = event.button.x;
					y = event.button.y;
					mouse_state=1;
				break;

				case SDL_MOUSEBUTTONUP:
					x = event.button.x;
					y = event.button.y;
					mouse_state=2;
				break;

				default:
				break;
			}
		}

		// cell previously under the mouse's cursor back to being displayed in it's original colour
		if(id_mouse_cell>=0 && id_mouse_cell<390){
			if(game->player->cell_tab[id_mouse_cell]->selection==MOUSE){
				game->player->cell_tab[id_mouse_cell]->selection=UNSELECT;
			}
		}

		// get the id of the cell under the mouse's cursor
		id_mouse_cell = get_cell_id_from_mouse_position(g, x, y, 0);

		// display the cell under the mouse's cursor in red
		if(mouse_state == 0){
			//if(is_in(g->board_player, x, y)){
			id_mouse_cell = get_cell_id_from_mouse_position(g, x, y, 0);
			if(id_mouse_cell >= 0 && id_mouse_cell<390){
				if(game->player->cell_tab[id_mouse_cell]->selection == UNSELECT){
					id_mouse_cell = get_cell_id_from_mouse_position(g, x, y, 0);
					game->player->cell_tab[id_mouse_cell]->selection = MOUSE;
				}
			}
			//}
		}
		// update
		if(mouse_state == 1){
		}

		if(mouse_state == 2){
		}

		// render
		//display_game(g, game);
		display_game(g, game);
		SDL_Delay(10);
	}

	///*
	SDL_RenderClear(g->renderer);

	IMG_Quit();

	end_sdl(1, "Normal ending", g->window, g->renderer);
	//*/

	for(int i = 0; i < 34; i++){
		free(deck->deck[i]);
	}

	free(deck);
}
