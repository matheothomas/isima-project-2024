/* name : utilities.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#ifndef utilities_h 
#define utilities_h

#include <stdbool.h>
#include <stdint.h>
#include "init.h"
/* Struct definitions */

/// Linked dtructure used to represent placing a tile on the board. It contains the coefficients for MCTS.
typedef struct play {
	tile_t * tile;
	int n_coup;
	float gain_coup;
	struct play * next;
} play_t;

/// Tree used for MCTS
typedef struct tree {
	play_t *play;
	struct tree *next;
} tree_t;

/// Wrapper for play_t that adds the size of the linked list
typedef struct linked_plays {
	int size;
	play_t * play;
} linked_plays_t;




/* Functions definitions */

/// Check if a tile with the correct pointers in cell_tab is valid
bool validity_tile(tile_t * tile_to_add);

/// declares a type_linked_t
type_linked_t * add_level(tile_t * tile, type_linked_t * level, cell_type_e cell_type);

/// Remove a level from a cell
void remove_level(cell_t * cell);

/// Update altitude and add level to all cells in the tile
void add_tile(tile_t * tile);

/// Inverse of add_tile and put pointers of cells in the tile to NULL
void undo_tile(tile_t * tile);

/// Same as undo_tile bus does not puts the pointers to NULL
void undo_without_null_tile(tile_t * tile);

/// Test if cell is on the edge
bool cell_in_periphery(cell_t * cell);

/// Test if cell has no neighbors of the same cell_type
bool cell_isolated(cell_t * cell, cell_type_e cell_type);

/// Test if cell is not on the edge
bool cell_circled(cell_t * cell);

/// Returns the size of the linked space corresponding to cell_type given
int get_connex_size(bool * visited, cell_t * cell, cell_type_e cell_type);

/// Same as get_connex_size but multiply result by the altitude of the cell
int get_connex_size_with_altitude(bool * visited, cell_t * cell, cell_type_e cell_type);

/// Return the maximum connex size coefficented with altitude
int maximum_connex_size_with_altitude(board_t * board, cell_type_e cell_type);

/// Like the title says
void calculate_score_from_table(board_t * board);

/// Update the value of the score table. Operation describes addition (1) and soustraction (-1)
void update_scoring_table(board_t * board);

/// Remove table from the board and puts pointers to NULL
void remove_tile_from_board(board_t * board, tile_t * tile);

/// Remove table from the board
void remove_tile_from_board_without_null(board_t * board, tile_t * tile);

/// Same
void add_tile_to_board(board_t * board, tile_t * tile);

/// Generate all plays associated to the current state of the board and the tile given
linked_plays_t * gen_tiles(cell_t ** cell_tab, tile_t * tile);

/// Fusion two linked_plays_t lists
linked_plays_t * fusion_linked_plays(linked_plays_t * linked_plays_1, linked_plays_t * linked_plays_2);

/// Generates all possible plays from two tiles
linked_plays_t * gen_tiles_from_game(game_t * game, bool is_bot);

/*
 * Deck functions
 */

/// Updates the deck.
void update_deck(game_t *game, tile_t *tile, bool is_bot);

/// Undo the deck.
void undo_deck(game_t *game, tile_t *tile, bool is_bot);


#endif

