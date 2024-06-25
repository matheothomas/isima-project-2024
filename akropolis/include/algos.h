/* name : algos.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#ifndef algos_h 
#define algos_h

#include "init.h"
#include "utilities.h"

/* Struct definitions */

/* Functions definitions */

/// itinialises the root of the tree with the two tiles available
play_t *initialisation(game_t *game, struct hash *h);

/// selects the play to play
play_t *selection(tree_t *tree);

///
void ucb();

///
void mcts();

///


#endif

