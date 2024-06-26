/* name : tests_eloi.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
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
#include <time.h>
#include "graphics.h"
#include "utilities.h"
#include "init.h"
#include "hash_map.h"
#include "tests_eloi.h"

void test_hash_index() {
	board_t * board = create_board();
	board -> score = 0;
	printf("hash index clean board %d\n", hash_board(board));
}


void test_add_tiles() {
	
    srand(time(0));

    graphics_t *g = init_sdl();
    
    // create/init game

    board_t *player=create_board();
    tile_t *first_player=create_tile();
    fill_tile(first_player, BLUE_PLACE, PURPLE_PLACE, RED_PLACE);
    first_player->cell_tab[0]=player->cell;
    first_player->cell_tab[1]=player->cell->neighbour[0];
    first_player->cell_tab[2]=player->cell->neighbour[1];
    // add_tile(first_player);
 
    board_t *bot=create_board();
    tile_t *first_bot=create_tile();
    fill_tile(first_bot, BLUE_PLACE,  QUARRY_GRAY, QUARRY_GRAY);
    first_bot->cell_tab[0]=bot->cell;
    first_bot->cell_tab[1]=bot->cell->neighbour[0];
    first_bot->cell_tab[2]=bot->cell->neighbour[1];

    deck_t *deck=create_deck();
    init_deck(deck);

    game_t *game=create_game();
    update_game(game, player, bot, deck->tile_tab[0], deck->tile_tab[1], deck);
	
	tile_t * tile = game -> deck -> tile_tab[3];
	tile -> cell_tab[0] = game -> player -> cell -> neighbour[0] -> neighbour[0];
	tile -> cell_tab[1] = game -> player -> cell -> neighbour[0] -> neighbour[0] -> neighbour[1];
	tile -> cell_tab[2] = game -> player -> cell -> neighbour[0] -> neighbour[0] -> neighbour[2];
	
	tile_t * tile2 = game -> deck -> tile_tab[4];
	tile2 -> cell_tab[0] = game -> player -> cell -> neighbour[5];
	tile2 -> cell_tab[1] = game -> player -> cell -> neighbour[4];
	tile2 -> cell_tab[2] = game -> player -> cell -> neighbour[4] -> neighbour[5];

	tile_t * tile3 = game -> deck -> tile_tab[5];
	tile3 -> cell_tab[0] = game -> player -> cell -> neighbour[0] -> neighbour[5];
	tile3 -> cell_tab[1] = game -> player -> cell -> neighbour[0] -> neighbour[5] -> neighbour[4];
	tile3 -> cell_tab[2] = game -> player -> cell -> neighbour[0] -> neighbour[5] -> neighbour[5];

	tile_t * tile4 = game -> deck -> tile_tab[6];
	tile4 -> cell_tab[0] = game -> player -> cell -> neighbour[5];
	tile4 -> cell_tab[1] = game -> player -> cell -> neighbour[5] -> neighbour[5];
	tile4 -> cell_tab[2] = game -> player -> cell -> neighbour[5] -> neighbour[0];

	tile_t * tile5 = game -> deck -> tile_tab[7];
	tile5 -> cell_tab[0] = game -> player -> cell -> neighbour[3];
	tile5 -> cell_tab[1] = game -> player -> cell -> neighbour[2];
	tile5 -> cell_tab[2] = game -> player -> cell -> neighbour[2] -> neighbour[3];

	tile_t * tile6 = game -> deck -> tile_tab[8];
	tile6 -> cell_tab[0] = game -> player -> cell -> neighbour[0];
	tile6 -> cell_tab[1] = game -> player -> cell -> neighbour[0] -> neighbour[0];
	tile6 -> cell_tab[2] = game -> player -> cell -> neighbour[0] -> neighbour[1];

	tile_t * tile7 = game -> deck -> tile_tab[9];
	tile7 -> cell_tab[0] = game -> player -> cell -> neighbour[0] -> neighbour[0];
	tile7 -> cell_tab[1] = game -> player -> cell -> neighbour[0] -> neighbour[0] -> neighbour[1];
	tile7 -> cell_tab[2] = game -> player -> cell -> neighbour[0] -> neighbour[0] -> neighbour[2];


	add_tile_to_board(game -> player, first_player);
	add_tile_to_board(game -> player, tile);
	add_tile_to_board(game -> player, tile2);
	add_tile_to_board(game -> player, tile3);
	add_tile_to_board(game -> player, tile4);
	add_tile_to_board(game -> player, tile5);
	// add_tile_to_board(game -> player, tile6);

	linked_plays_t * plays = gen_tiles_rec_false_start(game -> player, tile);
	play_t * cours = plays -> play;
	hash_t ** hash_map = create_hash_map();
	for (int i = 0; i < plays -> size; i++) {
		printf("Score before add : %d %d\n", game -> player -> score, game -> player -> table -> blue_mult);
		print_table(game -> player -> table);
		add_tile_to_board(game -> player, cours -> tile);
		printf("rocks %d\n", game -> player -> rocks);
		display_game(g, game);
		SDL_Delay(100);
		uint32_t hash = hash_board(game -> player);
		hash_map_add(hash_map, game -> player, plays);
		remove_tile_from_board(game -> player, cours -> tile);
		cours = cours -> next;
		printf("%d %d %zu %d\n", plays -> size, i, (unsigned long)hash, hash % HASHMAP_SIZE);

	}

	// remove_tile_from_board(game -> player, tile);



    // initialisation of variables

	// mouse's position and state
	int x = 0;
	int y = 0;
	(void)x;
	(void)y;
	int mouse_state = 0;

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
		}

		if(mouse_state==2){
		}

		// render
        //display_game(g, game);
        display_game(g, game);
		SDL_Delay(1);
	}

    ///*
	SDL_RenderClear(g->renderer);

	IMG_Quit();

	end_sdl(1, "Normal ending", g->window, g->renderer);
	//*/

    for(int i=0;i<34;i++){
        free(deck->tile_tab[i]);
    }

    free(deck);

}
