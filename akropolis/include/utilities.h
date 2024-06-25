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
#include "algos.h"
/* Struct definitions */
typedef struct hash {
	uint32_t hashed_board;
	play_t * plays;
	struct hash * next;
} hash_t;

typedef struct linked_plays {
	int size;
	play_t * play;
} linked_plays_t;

/* Functions definitions */
bool validity_tile(tile_t * tile_to_add);

type_linked_t * add_level(tile_t * tile, type_linked_t * level, cell_type_e cell_type);

void remove_level(cell_t * cell);

void add_tile(tile_t * tile);

void undo_tile(tile_t * tile);

linked_plays_t * gen_tiles(cell_t ** cell_tab, tile_t * tile);

hash_t ** create_hash_map();

uint32_t hash_board(board_t * board_t);

hash_t * create_linked_hash(uint32_t hashed_board, play_t * plays, hash_t * next);

/// Merge two plays scores in place
void merge_plays(play_t * play, play_t * new_play);

void hash_map_add(hash_t ** hash_map, board_t * board, play_t * plays);

void free_plays(play_t * plays);

void free_linked_plays(linked_plays_t * linked_plays);

void free_hash_list(hash_t * hash);

void free_hash_map(hash_t ** hash_map);

#endif

