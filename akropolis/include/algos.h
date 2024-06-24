/* name : algos.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#ifndef algos_h 
#define algos_h

#include "init.h"

/* Struct definitions */

typedef struct play {
	tile_t * tile;
	int n_coup;
	float gain_coup;
	struct play * next;
} play_t;

typedef struct tree {
	play_t *play;
	struct tree *next;
} tree_t;


/* Functions definitions */

/// itinialises the root of the tree with the two tiles available
void initialisation();

///
void ucb();

///
void mcts();

///


#endif

