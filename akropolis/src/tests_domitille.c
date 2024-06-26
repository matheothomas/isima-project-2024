/* name : tests_domitille.c
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

#include "init.h"
#include "utilities.h"
#include "algos.h"
#include "graphics.h"

void test_domi_2(){

    srand(time(0));
    
    board_t *player=create_board();

    /*
    for(int i=0;i<390;i++){
        printf("id : %d i : %p\n",b->cell_tab[i]->id,b->cell_tab[i]);
    }
    */

    //tile_t **tile_tab=creates_all_tiles();

    /*
    for(int i=0;i<34;i++){
        printf("%d %d %d id : %d\n",tile_tab[i]->cell_types[0], tile_tab[i]->cell_types[1], tile_tab[i]->cell_types[2], tile_tab[i]->id);
    }
    printf("\n");
    */
 
    tile_t *first_player=create_tile();
    fill_tile(first_player, BLUE_PLACE,  QUARRY_GRAY, QUARRY_GRAY);
    first_player->cell_tab[0]=player->cell;
    first_player->cell_tab[1]=player->cell->neighbour[0];
    first_player->cell_tab[2]=player->cell->neighbour[1];
    add_tile(first_player);
    
    /*
    for(int i=0;i<390;i++){
        printf("id : %d ad : %p type : %d id tile : %p\n",b->cell_tab[i]->id, b->cell_tab[i], b->cell_tab[i]->level->cell_type, b->cell_tab[i]->level->tile);
    }
    */
 
    board_t *bot=create_board();
    tile_t *first_bot=create_tile();
    fill_tile(first_bot, BLUE_PLACE,  QUARRY_GRAY, QUARRY_GRAY);
    first_bot->cell_tab[0]=bot->cell;
    first_bot->cell_tab[1]=bot->cell->neighbour[0];
    first_bot->cell_tab[2]=bot->cell->neighbour[1];

    deck_t *deck=create_deck();
    init_deck(deck);

    /*
    tile_t **deck=malloc(34* sizeof(tile_t *));

    for(int i=0;i<34;i++){
        int random =rand() % 34;
        if(tile_tab[random]) {
            deck[i] = tile_tab[random];
            tile_tab[random]=NULL;
        }
        else {
            i--;
        }
    }
    */

    for(int i=0;i<34;i++){
        printf("%d %d %d id : %d\n",deck->deck[i]->cell_types[0], deck->deck[i]->cell_types[1], deck->deck[i]->cell_types[2], deck->deck[i]->id);
    }

    game_t *game=create_game();
    update_game(game, player, bot, deck->deck[0], deck->deck[1], deck);

    for(int i=0;i<34;i++){
        free(deck->deck[i]);
    }

    free(deck);
}

void test_domi(){

    srand(time(0));

    graphics_t *g = init_sdl();
    
    // create/init game

    board_t *player=create_board();
    tile_t *first_player=create_tile();

    fill_tile(first_player, BLUE_PLACE,  QUARRY_GRAY, QUARRY_GRAY);
    first_player->cell_tab[0]=player->cell;
    first_player->cell_tab[1]=player->cell->neighbour[0];
    first_player->cell_tab[2]=player->cell->neighbour[1];

    add_tile_to_board(player, first_player);
 
    board_t *bot=create_board();
    tile_t *first_bot=create_tile();

    fill_tile(first_bot, BLUE_PLACE,  QUARRY_GRAY, QUARRY_GRAY);
    first_bot->cell_tab[0]=bot->cell;
    first_bot->cell_tab[1]=bot->cell->neighbour[0];
    first_bot->cell_tab[2]=bot->cell->neighbour[1];

    add_tile_to_board(bot, first_bot);

    deck_t *deck=create_deck();
    init_deck(deck);

    game_t *game=create_game();
    update_game(game, player, bot, deck->deck[0], deck->deck[1], deck);



    // initialisation of variables

	// mouse's position and state
	int x = 0;
	int y = 0;
	int mouse_state = 0;
    int id_mouse_cell=-1;
    bool is_bot_turn=0;

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

        if(is_bot_turn){
            game->selected_card=0;
            is_bot_turn=0;
        }
        else{
            // cell previously under the mouse's cursor back to being displayed in it's original colour
            if(id_mouse_cell>=0 && id_mouse_cell<390){
                if(game->player_board){
                    if(game->player->cell_tab[id_mouse_cell]->selection==MOUSE){
                        game->player->cell_tab[id_mouse_cell]->selection=UNSELECT;
                    }
                }
                else{
                    if(game->bot->cell_tab[id_mouse_cell]->selection==MOUSE){
                        game->bot->cell_tab[id_mouse_cell]->selection=UNSELECT;
                    }
                }
            }

            // get the id of the cell under the mouse's cursor
            id_mouse_cell = get_cell_id_from_mouse_position(g, x, y, 0);

            // display the selected tile up the cells under the mouse's cursor
            if(mouse_state == 0){
                //if(is_in(g->board_player, x, y)){
                    id_mouse_cell = get_cell_id_from_mouse_position(g, x, y, 0);
                    if(id_mouse_cell >= 0 && id_mouse_cell<390){
                        if(game->player_board){
                            if(game->player->cell_tab[id_mouse_cell]->selection == UNSELECT){
                                id_mouse_cell = get_cell_id_from_mouse_position(g, x, y, 0);
                                game->player->cell_tab[id_mouse_cell]->selection = MOUSE;
                            }
                        }
                        else{
                            if(game->bot->cell_tab[id_mouse_cell]->selection == UNSELECT){
                                id_mouse_cell = get_cell_id_from_mouse_position(g, x, y, 0);
                                game->bot->cell_tab[id_mouse_cell]->selection = MOUSE;
                            }
                        }
                    }
                //}
            }

            if(mouse_state==1){
            }

            if(mouse_state==2){
                if(is_in(g->first_tile, x, y)){
                    game->selected_card=1;
                }
                else if (is_in(g->second_tile, x, y)){
                    game->selected_card=2;
                }
                else if (is_in(g->left_arrow, x, y)) {
                    if(game->selected_card==1){
                        game->card_1->orientation=(game->card_1->orientation+5)%6;
                    }
                    else if(game->selected_card==2){
                        game->card_2->orientation=(game->card_2->orientation+5)%6;
                    }
                }
                else if (is_in(g->right_arrow, x, y)) {
                    if(game->selected_card==1){
                        game->card_1->orientation=(game->card_1->orientation+1)%6;
                    }
                    else if(game->selected_card==2){
                        game->card_2->orientation=(game->card_2->orientation+1)%6;
                    }
                }
                else if (is_in(g->mini_board, x, y)){
                    game->player_board=(game->player_board+1)%2;
                }
                else if (is_in(g->displayed_board, x, y)){
                    if(game->player_board){
                        if(game->selected_card==1){
                            update_tile_position(game->card_1, game->player->cell_tab[id_mouse_cell]);
                            if(validity_tile(game->card_1)){
                                add_tile_to_board(game->player, game->card_1);
                                update_deck(game, game->card_1);
                                is_bot_turn=1;
                            }
                        }
                        else if(game->selected_card==2){
                            update_tile_position(game->card_2, game->player->cell_tab[id_mouse_cell]);
                            if(validity_tile(game->card_2)){
                                add_tile_to_board(game->player, game->card_2);
                                update_deck(game, game->card_2);
                                is_bot_turn=1;
                            }
                        }
                    }           
                }
            }

        }

        

		// render
        display_game(g, game);
        //display_board(g, game->bot, g->board_bot->x);
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
