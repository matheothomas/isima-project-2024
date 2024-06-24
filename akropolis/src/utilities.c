/* name : utilities.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "algos.h"

bool validity_tile(tile_t * tile_to_add) {

	// Check if the tile is placed at the right level
	if (tile_to_add -> cell_tab[0] -> altitude != tile_to_add -> cell_tab[1] -> altitude ||
		tile_to_add -> cell_tab[0] -> altitude != tile_to_add -> cell_tab[2] -> altitude) {
		return false;
	}

	// Check if the tile is placed on intersection of two tiles by comparing the pointer of the uppermost tile
	if (tile_to_add -> cell_tab[0] -> level -> tile == 
		tile_to_add -> cell_tab[1] -> level -> tile &&
		tile_to_add -> cell_tab[0] -> level -> tile ==
		tile_to_add -> cell_tab[2] -> level -> tile) {
		return false;
	}

	int number_empty_neighbour = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 6; j++) {
			if (tile_to_add -> cell_tab[i] -> neighbour[j] == NULL ||
				tile_to_add -> cell_tab[i] -> neighbour[j] -> level -> cell_type == EMPTY) {
				number_empty_neighbour++;
			}
		}
	}
	// If the tile is all alone
	if (number_empty_neighbour == 9) {
		return false;
	}

	// Check if the new graph is not on the edge
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 6; j++) {
			if (tile_to_add -> cell_tab[i] -> neighbour[j] == NULL) {
				return false;
			}
		}
	}

	return true;
}

type_linked_t * add_level(tile_t * tile, type_linked_t * level, cell_type_e cell_type) {
	type_linked_t * type = malloc(sizeof(type_linked_t));
	type -> cell_type = cell_type;
	type -> tile = tile;
	type -> next = level;

	return type;
}

void remove_level(cell_t * cell) {
	type_linked_t * next = cell -> level -> next;
	free(cell -> level);
	cell -> level = next;
}

void add_tile(tile_t * tile) {
	for (int i = 0; i < 3; i++) {
		tile -> cell_tab[i] -> altitude ++;
		tile -> cell_tab[i] -> level = add_level(tile, tile -> cell_tab[i] -> level, tile -> cell_types[i]);
	}
}

void undo_tile(tile_t * tile) {
	for (int i = 0; i < 3; i++) {
		tile -> cell_tab[i] -> altitude --;
		remove_level(tile -> cell_tab[i]);
		tile -> cell_tab[i] = NULL;
	}
}

play_t * gen_tiles(cell_t ** cell_tab, tile_t * tile) {

	play_t * list = malloc(sizeof(play_t));
	list -> tile = NULL;
	list -> next = NULL;

	play_t * previous = list;

	for (int i = 0; i < CELL_NUMBER; i++) {
		for (int orientation = 0; orientation < 5; orientation++) {
			play_t * play = malloc(sizeof(play_t));
			play -> tile = NULL;
			play -> next = NULL;
			tile_t * new_tile = malloc(sizeof(tile_t));
			new_tile -> id = tile -> id;
			new_tile -> orientation = orientation;

			new_tile -> cell_tab[0] = cell_tab[i];
			new_tile -> cell_tab[1] = cell_tab[i] -> neighbour[orientation];
			new_tile -> cell_tab[2] = cell_tab[i] -> neighbour[(orientation + 1) % 6];

			new_tile -> cell_types[0] = tile -> cell_types[0];
			new_tile -> cell_types[1] = tile -> cell_types[1];
			new_tile -> cell_types[2] = tile -> cell_types[2];

			if (validity_tile(new_tile)) {
				play -> tile = new_tile;
				previous -> next = play;
			}
			else {
				free(new_tile);
				free(play);
			}
		}
	}
	return list;
}
