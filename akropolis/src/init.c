/* name : init.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/init.h"

cell_t *create_cell(){
    cell_t *cell=malloc(sizeof(cell_t));
    init_cell(cell);
    return cell;
}

void init_cell(cell_t * cell){
    cell->id=0;
	cell->cell_type=QUARRY_GRAY;
    for(int i=0;i<6;i++){
        cell->neighbour[i]=NULL;
    }
	cell->parent=NULL;
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

tile_t ** creates_all_tiles(){
    
}