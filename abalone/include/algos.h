/* name : algos.h
 * author : matheo thomas
 * date : 18-06-24
 */

#include <stdbool.h>
#include "init.h"

#ifndef algos_h 
#define algos_h

/* Struct definitions */

typedef struct play {
	cell_t *cell_tab[5];
	int cell_tab_length;
	int direction;
} play_t;

typedef struct tree {
	struct tree *tree_tab;
	play_t *play;
	int tab_length;
	int value;
	int depth;
} tree_t;


/* Functions definitions */

/// returns the best play depending on the player
play_t *choose_play(tree_t *tree, bool is_bot);

/// returns a tree containing all available plays for a board
tree_t *gen_plays(tree_t *tree, board_t *board);

/// applies a play to the board
tree_t *apply_play(tree_t *tree, board_t *board, play_t *play, bool is_bot);

/// applies the min-max algorithm 
tree_t *eval(tree_t *tree, board_t *board, int max_depth, bool is_bot);


#endif

