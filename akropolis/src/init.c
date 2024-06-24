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
    cell->x=0;
    cell->y=0;
	cell->altitude=0;
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

    int i=0;
    int k=0;
    cell_t *cur_cell=NULL;
    cell_t **cell_tab=(cell_t **)malloc(390*sizeof(cell_t*));

    // creates all cells and stock them in cell_tab
    for(k=0;k<390;k+=39){

        for(i=k;i<k+20;i++){
            cur_cell=create_cell();
            cur_cell->id=i;
            cell_tab[i]=cur_cell;
            cur_cell=cur_cell->neighbour[2];
        }

        for(i=k+20;i<k+39;i++){
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
            cell_tab[k]->neighbour[1]=cell_tab[k-1];
            cell_tab[k-1]->neighbour[4]=cell_tab[k];
        }
        for(i=k+1;i<k+19;i++){
            if(k){
                cell_tab[i]->neighbour[0]=cell_tab[2*k-i];
                cell_tab[2*k-i]->neighbour[3]=cell_tab[i];

                cell_tab[i]->neighbour[1]=cell_tab[2*k-i-1];
                cell_tab[2*k-i-1]->neighbour[4]=cell_tab[i];
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

        // i=k+20
        cell_tab[k+20]->neighbour[5]=cell_tab[k+21];

        for(i=k+20;i<k+39;i++){
            cell_tab[i]->neighbour[0]=cell_tab[2*(k+19)-i];
            cell_tab[2*(k+19)-i]->neighbour[3]=cell_tab[i];

            cell_tab[i]->neighbour[1]=cell_tab[2*(k+19)-i+1];
            cell_tab[2*(k+19)-i+1]->neighbour[4]=cell_tab[i];

            if(i!=k+20 && i!=k+38){
                cell_tab[i]->neighbour[5]=cell_tab[i+1];
                cell_tab[i]->neighbour[2]=cell_tab[i-1];
            }
        }
        // i=k+38
        cell_tab[k+38]->neighbour[2]=cell_tab[k+37];
    }

    board->cell=cell_tab[165];
    free(cell_tab);

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