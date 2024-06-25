/* name : utilities.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#ifndef utilities_h 
#define utilities_h

#define HASHMAP_SIZE 1000000


#include <stdbool.h>
#include <stdint.h>
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

typedef struct linked_plays {
	int size;
	play_t * play;
} linked_plays_t;

typedef struct hash {
	uint32_t hashed_board;
	linked_plays_t * plays;
	struct hash * next;
} hash_t;


/* Functions definitions */
bool validity_tile(tile_t * tile_to_add);

type_linked_t * add_level(tile_t * tile, type_linked_t * level, cell_type_e cell_type);

void remove_level(cell_t * cell);

void add_tile(tile_t * tile);

void undo_tile(tile_t * tile);

void undo_without_null_tile(tile_t * tile);

bool cell_in_periphery(cell_t * cell);

bool cell_isolated(cell_t * cell, cell_type_e cell_type);

bool cell_circled(cell_t * cell);

int get_connex_size(bool * visited, cell_t * cell, cell_type_e cell_type);

int get_connex_size_with_altitude(bool * visited, cell_t * cell, cell_type_e cell_type);

int maximum_connex_size_with_altitude(board_t * board, cell_type_e cell_type);

void calculate_score_from_table(board_t * board);

void update_scoring_table(board_t * board, tile_t * tile, int operation);

void remove_tile_from_board(board_t * board, tile_t * tile);

void add_tile_to_board(board_t * board, tile_t * tile);

linked_plays_t * gen_tiles(cell_t ** cell_tab, tile_t * tile);

linked_plays_t * fusion_linked_plays(linked_plays_t * linked_plays_1, linked_plays_t * linked_plays_2);

linked_plays_t * gen_tiles_from_game(game_t * game, bool is_bot);

uint32_t pow_u32(uint32_t x, int n);

hash_t ** create_hash_map();

uint32_t hash_board(board_t * board_t);

hash_t * create_linked_hash(uint32_t hashed_board, linked_plays_t * plays, hash_t * next);

/// Merge two plays scores in place
void merge_plays(play_t * play, play_t * new_play);

void hash_map_add(hash_t ** hash_map, board_t * board, linked_plays_t * plays);

void free_plays(play_t * plays);

void free_linked_plays(linked_plays_t * linked_plays);

void free_hash_list(hash_t * hash);

void free_hash_map(hash_t ** hash_map);

#endif

