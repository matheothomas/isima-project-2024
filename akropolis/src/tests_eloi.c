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
    fill_tile(first_player, BLUE_PLACE,  QUARRY_GRAY, QUARRY_GRAY);
    first_player->cell_tab[0]=player->cell;
    first_player->cell_tab[1]=player->cell->neighbour[0];
    first_player->cell_tab[2]=player->cell->neighbour[1];
    add_tile(first_player);
 
    board_t *bot=create_board();
    tile_t *first_bot=create_tile();
    fill_tile(first_bot, BLUE_PLACE,  QUARRY_GRAY, QUARRY_GRAY);
    first_bot->cell_tab[0]=bot->cell;
    first_bot->cell_tab[1]=bot->cell->neighbour[0];
    first_bot->cell_tab[2]=bot->cell->neighbour[1];

    deck_t *deck=create_deck();
    init_deck(deck);

    game_t *game=create_game();
    update_game(game, player, bot, deck->deck[0], deck->deck[1], deck);
	
	printf("");
	tile_t * tile = game -> deck -> deck[3];
	tile -> cell_tab[0] = game -> player -> cell -> neighbour[0] -> neighbour[0];
	tile -> cell_tab[1] = game -> player -> cell -> neighbour[0] -> neighbour[0] -> neighbour[2];
	tile -> cell_tab[2] = game -> player -> cell -> neighbour[3] -> neighbour[0] -> neighbour[3];
	add_tile_to_board(game -> player, tile);
	remove_tile_from_board(game -> player, tile);



    // initialisation of variables

	// mouse's position and state
	int x = 0;
	int y = 0;
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
        display_board(g, game);
		SDL_Delay(1);
	}

    ///*
	SDL_RenderClear(g->renderer);

	IMG_Quit();

	end_sdl(1, "Normal ending", g->window, g->renderer);
	//*/

    for(int i=0;i<34;i++){
        free(deck->deck[i]);
    }

    free(deck);

}
