/* name : tests_matheo.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdio.h>
#include <stdlib.h>
#include "hash_map.h"



// Pow with exponentation by squaring
uint32_t pow_u32(uint32_t x, int n) {
	uint32_t res = 1;
	while (1) {
		if (n & 1) {
			res *= x;
		}
		// equivalent to (n - 1) / 2
		n >>= 1;
		// if n is zero
		if (!n) {
			break;
		}
		x *= x;
	}

	return res;
}

hash_t ** create_hash_map() {

	hash_t ** hash_map = malloc(sizeof(hash_t *) * HASHMAP_SIZE);
	for (int i = 0; i < HASHMAP_SIZE; i++) {
		hash_map[i] = NULL;
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

hash_t * create_linked_hash(uint32_t hashed_board, linked_plays_t * plays, hash_t * next) {
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

void hash_map_add(hash_t ** hash_map, board_t * board, linked_plays_t * plays) {
	uint32_t hashed_board = hash_board(board);
	int hash_index = hashed_board % HASHMAP_SIZE;

	hash_t * hash = hash_map[hash_index];

	if (hash == NULL) {
		hash_map[hash_index] = create_linked_hash(hashed_board, plays, NULL);
		return;
	}

	while (hash -> next != NULL) {
		if (hash -> hashed_board == hashed_board) {
			merge_plays(hash -> plays -> play, plays -> play);
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
		free_linked_plays(previous -> plays);
		free(previous);
	}
}

void free_hash_map(hash_t ** hash_map) {

	for (int i = 0; i < HASHMAP_SIZE; i++) {
		free_hash_list(hash_map[i]);
	}
	free(hash_map);
}
