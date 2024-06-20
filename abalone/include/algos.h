/* name : algos.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 18-06-24
 */

#include <stdbool.h>

#include "init.h"
#include "graphics.h"

#ifndef algos_h 
#define algos_h

/* Struct definitions */

typedef struct play {
	cell_t *cell_tab[5];
	state_e buffer[5];
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

/// returns the max/min value of a tree node, depending on the player turn
int max(tree_t *tree, bool player);

/// return the max play of a tree node
play_t *max_play(tree_t *tree);

///
int basic_heuristic(cell_t **cell_tab);

/// returns the best play depending on the player
// play_t *choose_play(board_t *board, graphics_t *g, cell_t **cell_tab);
play_t *choose_play(board_t *board, cell_t **cell_tab, bool player);

/// applies a play to the board
board_t *apply_play(board_t *board, play_t *play);

///
board_t *undo_play(board_t *board, play_t *play);

/// applies the min-max algorithm 
int eval(board_t *board, cell_t **cell_tab, int depth, int max_depth, bool player);


#endif

