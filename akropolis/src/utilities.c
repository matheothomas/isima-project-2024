/* name : utilities.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include "init.h"
#include "utilities.h"

/*
 * Checking and generating plays
 */

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



void add_tile_to_board(board_t * board, tile_t * tile) {

	add_tile(tile);

	for (int i = 0; i < 3; i++) {
		switch (tile -> cell_types[i]) {
			case EMPTY:
				fprintf(stderr, "Tile should not contain EMPTY value %d\n", i);
			break;
			case HOUSE_BLUE:

			break;
			case BARRAK_RED:
			break;
			case MARKET_YELLOW:
			break;
			case TEMPLE_PURPLE:
			break;
			case PARK_GREEN:
				
			break;
			case QUARRY_GRAY:
			break;
			case BLUE_PLACE:
			break;
			case YELLOW_PLACE:
			break;
			case RED_PLACE:
			break;
			case PURPLE_PLACE:
			break;
			case GREEN_PLACE:
			break;
		}
	}
}

void undo_tile(tile_t * tile) {
	for (int i = 0; i < 3; i++) {
		tile -> cell_tab[i] -> altitude --;
		remove_level(tile -> cell_tab[i]);
		tile -> cell_tab[i] = NULL;
	}
}

linked_plays_t * gen_tiles(cell_t ** cell_tab, tile_t * tile) {

	play_t * list = malloc(sizeof(play_t));
	list -> tile = NULL;
	list -> next = NULL;

	linked_plays_t * linked_plays = malloc(sizeof(linked_plays_t));
	linked_plays -> size = 0;
	linked_plays -> play = list;

	play_t * previous = list;

	int size = 0;

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
				size++;
			}
			else {
				free(new_tile);
				free(play);
			}
		}
	}

	return linked_plays;
}

/*
 * Hashmaps functions
 */

// Pow with exponentation by squaring
uint32_t pow_u32(uint32_t x, int n) {
	uint32_t res = 1;
	while (1) {
		if (n & 1)
			res *= x;
		// equivalent to (n - 1) / 2
		n >>= 1;
		// if n is zero
		if (!n)
			break;
		x *= x;
	}

	return res;
}

hash_t ** create_hash_map() {

	hash_t ** hash_map = malloc(sizeof(hash_t *) * HASHMAP_SIZE);
	for (int i = 0; i < HASHMAP_SIZE; i++) {
		hash_map[i] = calloc(HASHMAP_SIZE, sizeof(hash_t));
	}

	return hash_map;
}

uint32_t hash_board(board_t * board) {

	uint32_t nb_tiles = 0;
	uint32_t nb_cells = 0;
	uint32_t sum_cells_ids = 0;
	uint32_t sum_cells_values = 0;
	uint32_t sum_orientation_id_tiles = 0;

	cell_t ** cell_tab = board -> cell_tab;

	for (int i = 0; i < CELL_NUMBER; i++) {
		if (cell_tab[i] -> level -> cell_type != EMPTY) {
			nb_cells++;
			nb_tiles++;
			sum_cells_ids += cell_tab[i] -> id;
			sum_cells_values += cell_tab[i] -> level -> cell_type;
			sum_orientation_id_tiles += cell_tab[i] -> level -> tile -> orientation * cell_tab[i] -> level -> tile -> id;
		}
	}

	uint32_t hash = 31 * board -> score + pow_u32(31, 2) * nb_tiles + pow_u32(31, 3) * nb_cells + pow_u32(31, 4) * sum_orientation_id_tiles + pow_u32(31, 5) * sum_cells_values + pow_u32(31, 6) * sum_cells_ids;

	return hash;
}

hash_t * create_linked_hash(uint32_t hashed_board, play_t * plays, hash_t * next) {
	hash_t * new_hash = malloc(sizeof(hash_t));
	new_hash -> hashed_board = hashed_board;
	new_hash -> plays = plays;
	new_hash -> next = next;

	return new_hash;
}

void merge_plays(play_t * play, play_t * new_play) {

	while (play != NULL && new_play != NULL) {
		play -> n_coup += new_play -> n_coup;
		play -> gain_coup += new_play -> gain_coup;
		play = play -> next;
		new_play = new_play -> next;
	}
	if ((play != NULL) ^ (new_play != NULL)) {
		fprintf(stderr, "play or new_play may be of different size play : %p new_play %p\n", play, new_play);
	}
}

void hash_map_add(hash_t ** hash_map, board_t * board, play_t * plays) {
	uint32_t hashed_board = hash_board(board);
	int hash_index = hashed_board / HASHMAP_SIZE;

	hash_t * hash = hash_map[hash_index];

	if (hash == NULL) {
		hash_map[hash_index] = create_linked_hash(hashed_board, plays, NULL);
		return;
	}

	while (hash -> next != NULL) {
		if (hash -> hashed_board == hashed_board) {
			merge_plays(hash -> plays, plays);
			return;
		}
		else {
			hash = hash -> next;
		}
	}
	hash -> next = create_linked_hash(hashed_board, plays, NULL);
}

void free_plays(play_t * plays) {

	play_t * previous = plays;

	while (plays != NULL) {
		previous = plays;
		plays = plays -> next;
		free(previous);
	}
}

void free_linked_plays(linked_plays_t * linked_plays) {
	free_plays(linked_plays -> play);
	free(linked_plays);
}

void free_hash_list(hash_t * hash) {

	hash_t * previous = hash;

	while (hash != NULL) {
		previous = hash;
		hash = hash -> next;
		free_plays(previous -> plays);
		free(hash);
	}
}

void free_hash_map(hash_t ** hash_map) {

	for (int i = 0; i < HASHMAP_SIZE; i++) {
		free_hash_list(hash_map[i]);
	}
	free(hash_map);
}
