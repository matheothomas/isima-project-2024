/* name : main.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 18-06-24
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include "algos.h"
#include "init.h"


/* Functions definitions */

/// prints the content of a play for debugging purposes
void print_play(play_t *play);

/// tests if two plays are identical
bool is_duplicate(play_t * play1, play_t * play2);

/// tests if a play is valid
bool validity_play(play_t * play, bool player);

/// creats a tree
tree_t * create_tree(play_t * play, int value, int depth);

/// frees a tree
void free_tree(tree_t * tree);

/// adds a node to the linked list tree
void append_tree(tree_t * tree, play_t * play, int value, int depth);

/// fills the buffer of a play
void fill_play_buffer(play_t * play);

/// part of the traversal_rec function, case where the following cell of a play belongs to the player
void cell_belongs_to_player(tree_t * tree, play_t * play, cell_t * cell, bool * visited, bool player);

/// part of the traversal_rec function, case where the following cell of a play does not belong to the player
void cell_does_not_belongs_to_player(tree_t * tree, play_t * play, cell_t * cell, bool * visited, bool player);

/// recursively generates plays
void traversal_rec(tree_t * tree, play_t * play, cell_t * cell, bool * visited, bool player);

/// generates all valid plays available given a board and a player
tree_t * gen_plays(board_t * board, int depth, bool player);


#endif
