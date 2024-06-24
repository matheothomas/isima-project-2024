/* name : init.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/init.h"

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
	cell->cell_type=create_type_linked();
    for(int i=0;i<6;i++){
        cell->neighbour[i]=NULL;
    }
	//cell->parent=NULL;
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
    }
	tile->orientation=0;
}

board_t *create_board(){
    board_t *board=malloc(sizeof(board_t));
    init_board(board);

    board->cell=create_cell();
    

    return board;
}

void init_board(board_t * board){
    board->cell=NULL;
	board->blue_mult=0;
	board->yellow_mult=0;
	board->red_mult=0;
	board->purple_mult=0;
	board->green_mult=0;
	board->rocks=0;	
}