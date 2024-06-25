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

void test_domi(){

    srand(time(0));

    //graphics_t *g = init_sdl();
    
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
    update_game(game, player, bot, deck->deck[0], deck->deck[1], deck->deck);

    for(int i=0;i<34;i++){
        free(deck->deck[i]);
    }

    free(deck);
}
