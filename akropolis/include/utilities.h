/* name : utilities.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#ifndef utilities_h 
#define utilities_h

#include "init.h"
#include <stdbool.h>
/* Struct definitions */



/* Functions definitions */
bool validity_tile(tile_t * tile_to_add);

type_linked_t * add_level(tile_t * tile, type_linked_t * level, cell_type_e cell_type);

void remove_level(cell_t * cell);

void add_tile(tile_t * tile);

void undo_tile(tile_t * tile);

#endif

