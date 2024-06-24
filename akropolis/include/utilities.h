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
	uint32_t hash;
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

uint32_t hash_index(cell_t ** cell_tab);

#endif

