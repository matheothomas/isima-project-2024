





#ifndef hash_map_h
#define hash_map_h

#define HASHMAP_SIZE 1000000

#include <stdbool.h>
#include <stdint.h>
#include "utilities.h"
#include "init.h"

/* Struct definitions */

/// Linked list for the hash_map. It contains the unmoduled hash of the board and the list of possible plays associated to this board.
typedef struct hash {
	uint32_t hashed_board;
	linked_plays_t * plays;
	struct hash * next;
} hash_t;

/* Functions definitions */

/// Calculate power for uint32_t type
uint32_t pow_u32(uint32_t x, int n);

/// Generate an empty hash_map
hash_t ** create_hash_map();

/// Return the unmoduled hash of a board
uint32_t hash_board(board_t * board_t);

/// Initialize and fills the hash_t structure
hash_t * create_linked_hash(uint32_t hashed_board, linked_plays_t * plays, hash_t * next);

/// Merge two plays scores in place
void merge_plays(play_t * play, play_t * new_play);

/// Adds a board to the hash_map
void hash_map_add(hash_t ** hash_map, board_t * board, linked_plays_t * plays);

/// Free play_t list
void free_plays(play_t * plays);

/// Free linked_play_t list
void free_linked_plays(linked_plays_t * linked_plays);

/// Free all the hash in a hash_map entry
void free_hash_list(hash_t * hash);

/// Free the hash_map
void free_hash_map(hash_t ** hash_map);

#endif

