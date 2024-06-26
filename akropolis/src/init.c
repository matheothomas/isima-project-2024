/* name : init.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdio.h>
#include <stdlib.h>

#include "init.h"
#include "algos.h"

type_linked_t *create_type_linked(){
    type_linked_t *type_linked=malloc(sizeof(type_linked_t));
    init_type_linked(type_linked);
    return type_linked;
}

void init_type_linked(type_linked_t * type_linked){
    type_linked->cell_type=EMPTY;
	type_linked->tile=NULL;
	type_linked->next=NULL;
}

cell_t *create_cell(){
    cell_t *cell=malloc(sizeof(cell_t));
    init_cell(cell);
    return cell;
}

void init_cell(cell_t * cell){
    cell->id=0;
	cell->level=create_type_linked();
    for(int i=0;i<6;i++){
        cell->neighbour[i]=NULL;
    }
    cell->x=0;
    cell->y=0;
	cell->altitude=0;
    cell->selection=UNSELECT;
	//cell->parent=NULL;
}

board_t *create_board(){
    board_t *board=malloc(sizeof(board_t));
    init_board(board);

    cell_t *cur_cell=NULL;
    cell_t **cell_tab=(cell_t **)malloc(390*sizeof(cell_t*));

    int i=0;
    int k=0;

    // creates all cells and stock them in cell_tab
    for(k=0;k<390;k+=39){

        for(i=k;i<k+20;i++){
            cur_cell=create_cell();
            cur_cell->id=i;
            cell_tab[i]=cur_cell;
            cur_cell=cur_cell->neighbour[2];
        }

        for(i=k+38;i>k+19;i--){
            cur_cell=create_cell();
            cur_cell->id=i;
            cell_tab[i]=cur_cell;
            cur_cell=cur_cell->neighbour[5];
        }
    }

    // creates link between cells
    for(k=0;k<390;k+=39){
        // i=k
        cell_tab[k]->neighbour[2]=cell_tab[k+1];
        if(k){
            cell_tab[k]->neighbour[1]=cell_tab[k-19];
            cell_tab[k-19]->neighbour[4]=cell_tab[k];
        }
        for(i=k+1;i<k+19;i++){
            if(k){
                cell_tab[i]->neighbour[0]=cell_tab[i-20];
                cell_tab[i-20]->neighbour[3]=cell_tab[i];

                cell_tab[i]->neighbour[1]=cell_tab[i-19];
                cell_tab[i-19]->neighbour[4]=cell_tab[i];
            }
            cell_tab[i]->neighbour[2]=cell_tab[i+1];
            cell_tab[i]->neighbour[5]=cell_tab[i-1];
        }
        // i=k+19
        cell_tab[k+19]->neighbour[5]=cell_tab[k+18];
        if(k){
            cell_tab[k+19]->neighbour[0]=cell_tab[k-19];
            cell_tab[k-19]->neighbour[3]=cell_tab[k+19];
        }

        // i=k+38
        cell_tab[k+38]->neighbour[5]=cell_tab[k+37];

        for(i=k+37;i>k+20;i--){

            cell_tab[i]->neighbour[0]=cell_tab[i-20];
            cell_tab[i-20]->neighbour[3]=cell_tab[i];

            cell_tab[i]->neighbour[1]=cell_tab[i-19];
            cell_tab[i-19]->neighbour[4]=cell_tab[i];

            if(i!=k+20 && i!=k+38){
                cell_tab[i]->neighbour[5]=cell_tab[i-1];
                cell_tab[i]->neighbour[2]=cell_tab[i+1];
            }
        }
        // i=k+20
        cell_tab[k+20]->neighbour[2]=cell_tab[k+21];
    }

    board->cell=cell_tab[165];
    board->cell_tab=cell_tab;

    return board;
}

void init_board(board_t * board){
    board->cell=NULL;
    board->table=init_scoring_table();
    board->score=0;
	board->rocks=0;
    board->cell_tab=NULL;
}

scoring_table_t *init_scoring_table(){
    scoring_table_t *scoring_table=malloc(sizeof(scoring_table_t));
    scoring_table->blue_mult=0;
    scoring_table->blue_nb_alt=0;
    scoring_table->yellow_mult=0;
    scoring_table->yellow_nb_alt=0;
    scoring_table->red_mult=0;
    scoring_table->red_nb_alt=0;
    scoring_table->purple_mult=0;
    scoring_table->purple_nb_alt=0;
    scoring_table->green_mult=0;
    scoring_table->green_nb_alt=0;
    return scoring_table;
}

tile_t *create_tile(){
    tile_t *tile=malloc(sizeof(tile_t));
    init_tile(tile);
    return tile;
}

void init_tile(tile_t * tile){
    tile->id=0;
    for(int i=0; i<3; i++){
        tile->cell_tab[i]=NULL;
        tile->cell_types[i]=QUARRY_GRAY;
    }
	tile->orientation=0;
}

void fill_tile(tile_t *tile, cell_type_e type_0, cell_type_e type_1, cell_type_e type_2){
    tile->cell_types[0]=type_0;
    tile->cell_types[1]=type_1;
    tile->cell_types[2]=type_2;
}

deck_t *create_deck(){
    deck_t *deck=malloc(sizeof(deck_t));
    deck->n=0;
    deck->deck=NULL;
    return deck;
}

void init_deck(deck_t *struct_deck){
    tile_t **deck = malloc(34* sizeof(tile_t *));
    tile_t **tile_tab = creates_all_tiles();
    for(int i=0;i<34;i++){
        int random = rand() % 34;
        if(tile_tab[random]) {
            deck[i] = tile_tab[random];
            tile_tab[random] = NULL;
        }
        else {
            i--;
        }
    }
    free(tile_tab);
    struct_deck->deck=deck;
}

tile_t ** creates_all_tiles(){
    tile_t **tile_tab=malloc(37*sizeof(tile_t*));
    int i=0;
    //for (i=0;i<3;i++){
        tile_tab[i]=create_tile();
        init_tile(tile_tab[i]);
        fill_tile(tile_tab[i], BLUE_PLACE, QUARRY_GRAY, QUARRY_GRAY);
        tile_tab[i]->id=i;
    //}
    for (i=1;i<5;i++){
        tile_tab[i]=create_tile();
        init_tile(tile_tab[i]);
        fill_tile(tile_tab[i], i+7, QUARRY_GRAY, QUARRY_GRAY);
        tile_tab[i]->id=i;
    }
    for (i=5;i<8;i++){
        tile_tab[i]=create_tile();
        init_tile(tile_tab[i]);
        fill_tile(tile_tab[i], i-2, QUARRY_GRAY, QUARRY_GRAY);
        tile_tab[i]->id=i;
    }

    i=8;
    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], HOUSE_BLUE, QUARRY_GRAY, RED_PLACE);
    tile_tab[i]->id=i;
    i++;
    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], HOUSE_BLUE, QUARRY_GRAY, MARKET_YELLOW);
    tile_tab[i]->id=i;
    i++;
    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], HOUSE_BLUE, QUARRY_GRAY, HOUSE_BLUE);
    tile_tab[i]->id=i;
    i++;
    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], HOUSE_BLUE, QUARRY_GRAY, PURPLE_PLACE);
    tile_tab[i]->id=i;
    i++;

    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], HOUSE_BLUE, BARRAK_RED, QUARRY_GRAY);
    tile_tab[i]->id=i;
    i++;
    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], HOUSE_BLUE, BARRAK_RED, TEMPLE_PURPLE);
    tile_tab[i]->id=i;
    i++;
    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], HOUSE_BLUE, BARRAK_RED, PURPLE_PLACE);
    tile_tab[i]->id=i;
    i++;

    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], HOUSE_BLUE, PARK_GREEN, MARKET_YELLOW);
    tile_tab[i]->id=i;
    i++;
    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], HOUSE_BLUE, PARK_GREEN, YELLOW_PLACE);
    tile_tab[i]->id=i;
    i++;
    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], HOUSE_BLUE, PARK_GREEN, BARRAK_RED);
    tile_tab[i]->id=i;
    i++;

    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], BARRAK_RED, QUARRY_GRAY, MARKET_YELLOW);
    tile_tab[i]->id=i;
    i++;
    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], BARRAK_RED, QUARRY_GRAY, TEMPLE_PURPLE);
    tile_tab[i]->id=i;
    i++;
    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], BARRAK_RED, QUARRY_GRAY, YELLOW_PLACE);
    tile_tab[i]->id=i;
    i++;

    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], MARKET_YELLOW, QUARRY_GRAY, TEMPLE_PURPLE);
    tile_tab[i]->id=i;
    i++;

    //for(int k=0;k<2;k++){
        tile_tab[i]=create_tile();
        init_tile(tile_tab[i]);
        fill_tile(tile_tab[i], MARKET_YELLOW, QUARRY_GRAY, BARRAK_RED);
        tile_tab[i]->id=i;
        i++;
    //}

    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], PARK_GREEN, QUARRY_GRAY, MARKET_YELLOW);
    tile_tab[i]->id=i;
    i++;
    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], PARK_GREEN, QUARRY_GRAY, BLUE_PLACE);
    tile_tab[i]->id=i;
    i++;

    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], QUARRY_GRAY, HOUSE_BLUE, GREEN_PLACE);
    tile_tab[i]->id=i;
    i++;
    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], QUARRY_GRAY, HOUSE_BLUE, PURPLE_PLACE);
    tile_tab[i]->id=i;
    i++;

    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], HOUSE_BLUE, TEMPLE_PURPLE, RED_PLACE);
    tile_tab[i]->id=i;
    i++;
    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], HOUSE_BLUE, TEMPLE_PURPLE, MARKET_YELLOW);
    tile_tab[i]->id=i;
    i++;

    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], GREEN_PLACE, HOUSE_BLUE, BARRAK_RED);
    tile_tab[i]->id=i;
    i++;
    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], GREEN_PLACE, HOUSE_BLUE, QUARRY_GRAY);
    tile_tab[i]->id=i;
    i++;

    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], MARKET_YELLOW, HOUSE_BLUE, RED_PLACE);
    tile_tab[i]->id=i;
    i++;

    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], MARKET_YELLOW, TEMPLE_PURPLE, QUARRY_GRAY);
    tile_tab[i]->id=i;
    i++;

    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], MARKET_YELLOW, BARRAK_RED, BLUE_PLACE);
    tile_tab[i]->id=i;
    i++;

    tile_tab[i]=create_tile();
    init_tile(tile_tab[i]);
    fill_tile(tile_tab[i], YELLOW_PLACE, TEMPLE_PURPLE, HOUSE_BLUE);
    tile_tab[i]->id=i;

    return tile_tab;
}

args_t *create_and_init_args(){
    args_t *args=malloc(sizeof(args_t));
    args->tile=NULL;
    args->cell=NULL;
    args->score=0;
    return args;
}

void updates_args(args_t *args, tile_t *tile, cell_t *cell, int score){
    args->cell=cell;
    args->tile=tile;
    args->score=score;
}

game_t * create_game(){
    game_t *game=malloc(sizeof(game_t));
    game->card_1=NULL;
    game->card_2=NULL;
    game->deck=NULL;
    game->player=NULL;
    game->bot=NULL;
    game->selected_card=0;
    return game;
}

void update_game(game_t *game, board_t *player, board_t *bot, tile_t *card_1, tile_t *card_2, deck_t *deck){
    game->card_1=card_1;
    game->card_2=card_2;
    game->deck=deck;
    game->player=player;
    game->bot=bot;
}



struct play * create_play(){
    struct play *play=malloc(sizeof(struct play));
    play->tile=NULL;
    play->n_coup=0;
    play->gain_coup=0;
    play->next=NULL;
    return play;
}

void update_play(struct play *play, tile_t *tile, int n_coup, float gain_coup, play_t *next){
    play->tile=tile;
    play->n_coup=n_coup;
    play->gain_coup=gain_coup;
    play->next=next;
}
