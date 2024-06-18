/* name : algos.h
 * author : matheo thomas
 * date : 18-06-24
 */

#include "init.h"
#ifndef algos_h 
#define algos_h

/* Struct definitions */

typedef struct play {
	cell_t *c;
	int direction;
} play_t;

typedef struct tree {
	struct tree *tree_tab;
	play_t play;
	int tab_length;
	int value;
	int depth;
} tree_t;


/* Functions definitions */


#endif

