/* name : tests_domitille.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/init.h"

void test_domi(){
    board_t *b=create_board();
    for(int i=0;i<390;i++){
        printf("id : %d i : %d\n",b->cell_tab[i]->id,i);
    }
}
