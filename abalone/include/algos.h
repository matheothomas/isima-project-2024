/* name : algos.h
 * author : matheo thomas
 * date : 18-06-24
 */

#include "init.h"
#ifndef algos_h 
#define algos_h

/* Struct definitions */

typedef struct play {
	cell_t *cell_tab[5];
	int buffer[5];
	int cell_tab_length;
	int movement_direction;
	int cell_direction;
} play_t;

typedef struct tree {
	struct tree *next_tree;
	play_t *play;
	int value;
	int depth;
} tree_t;


/* Functions definitions */


#endif

