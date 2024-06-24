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
	struct play * next;
} play_t;

/* Functions definitions */

/// itinialises the root of the tree with the two tiles available
void initialisation();

///
void ucb();

///
void mcts();

///


#endif

