/* name : tests_domitille.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/init.h"

void test_domi(){
    /*
    board_t *b=create_board();
    for(int i=0;i<390;i++){
        printf("id : %d i : %p\n",b->cell_tab[i]->id,b->cell_tab[i]);
    }
    */
    tile_t **tile_tab=creates_all_tiles();
    for(int i=0;i<37;i++){
        printf("%d %d %d id : %d\n",tile_tab[i]->cell_types[0], tile_tab[i]->cell_types[1], tile_tab[i]->cell_types[2], tile_tab[i]->id);
    }
}
