/* name : main.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 18-06-24
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include "init.h"
#include "algos.h"

/* Struct definitions */

/* Functions definitions */
void print_play(play_t *play);
bool is_duplicate(play_t * play1, play_t * play2);
bool validity_play(play_t * play, bool player);
void free_tree(tree_t * tree);
tree_t * create_tree(play_t * play, int value, int depth);
void append_tree(tree_t * tree, play_t * play, int value, int depth, bool player);
void fill_play_buffer(play_t * play);
void cell_belongs_to_player(board_t * board, tree_t * tree, play_t * play, cell_t * cell, bool * visited, bool player);
void cell_does_not_belongs_to_player(board_t * board, tree_t * tree, play_t * play, cell_t * cell, bool * visited, bool player);
void traversal_rec(board_t * board, tree_t * tree, play_t * play, cell_t * cell, bool * visited, bool player);
tree_t * gen_plays(board_t * board, int depth, bool player);

/// creates a cell of the board

#endif
