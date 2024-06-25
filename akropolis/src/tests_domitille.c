/* name : tests_domitille.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdio.h>
#include <stdlib.h>

#include "init.h"
#include "utilities.h"

void test_domi(){
    
    board_t *b=create_board();
    /*
    for(int i=0;i<390;i++){
        printf("id : %d i : %p\n",b->cell_tab[i]->id,b->cell_tab[i]);
    }
    */
    tile_t **tile_tab=creates_all_tiles();
    /*
    for(int i=0;i<34;i++){
        printf("%d %d %d id : %d\n",tile_tab[i]->cell_types[0], tile_tab[i]->cell_types[1], tile_tab[i]->cell_types[2], tile_tab[i]->id);
    }
    */
    tile_t *first=create_tile();
    fill_tile(first, BLUE_PLACE,  QUARRY_GRAY, QUARRY_GRAY);
    first->cell_tab[0]=b->cell;
    first->cell_tab[1]=b->cell->neighbour[0];
    first->cell_tab[2]=b->cell->neighbour[1];
    add_tile(first);
    /*
    for(int i=0;i<390;i++){
        printf("id : %d ad : %p type : %d id tile : %p\n",b->cell_tab[i]->id, b->cell_tab[i], b->cell_tab[i]->level->cell_type, b->cell_tab[i]->level->tile);
    }
    */
}
