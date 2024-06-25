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

void undo_tile(tile_t * tile) {
	for (int i = 0; i < 3; i++) {
		tile -> cell_tab[i] -> altitude --;
		remove_level(tile -> cell_tab[i]);
		tile -> cell_tab[i] = NULL;
	}
}

void undo_without_null_tile(tile_t * tile) {
	for (int i = 0; i < 3; i++) {
		tile -> cell_tab[i] -> altitude --;
		remove_level(tile -> cell_tab[i]);
		tile -> cell_tab[i] = NULL;
	}
}


void add_tile(tile_t * tile) {
	for (int i = 0; i < 3; i++) {
		tile -> cell_tab[i] -> altitude ++;
		tile -> cell_tab[i] -> level = add_level(tile, tile -> cell_tab[i] -> level, tile -> cell_types[i]);
	}
}

bool cell_in_periphery(cell_t * cell) {

	for (int i = 0; i < 6; i++) {
		if (cell -> neighbour[i] == NULL || cell -> neighbour[i] -> level -> cell_type == EMPTY) {
			return true;
		}
	}
	return false;
}

bool cell_isolated(cell_t * cell, cell_type_e cell_type) {

	for (int i = 0; i < 6; i++) {
		if (cell -> neighbour[i] != NULL && cell -> neighbour[i] -> level -> cell_type == cell_type) {
			return false;
		}
	}
	return true;
}

bool cell_circled(cell_t * cell) {

	for (int i = 0; i < 6; i++) {
		if (cell -> neighbour[i] == NULL || cell -> neighbour[i] -> level -> cell_type == EMPTY) {
			return false;
		}
	}
	return true;
}

int get_connex_size(bool * visited, cell_t * cell, cell_type_e cell_type) {
	
	visited[cell -> id] = true;
	int size = 1;
	for (int i = 0; i < 6; i++) {
		if (!visited[cell -> neighbour[i] -> id] && cell -> neighbour[i] -> level -> cell_type == cell_type) {
			size += get_connex_size(visited, cell -> neighbour[i], cell_type);
		}
	}
	return size;
}

int get_connex_size_with_altitude(bool * visited, cell_t * cell, cell_type_e cell_type) {
	
	visited[cell -> id] = true;
	int size_alt = cell -> altitude;
	for (int i = 0; i < 6; i++) {
		if (!visited[cell -> neighbour[i] -> id] && cell -> neighbour[i] -> level -> cell_type == cell_type) {
			size_alt += get_connex_size_with_altitude(visited, cell -> neighbour[i], cell_type);
		}
	}
	return size_alt;
}

int maximum_connex_size_with_altitude(board_t * board, cell_type_e cell_type) {

	int maxi_connex_size = 0;
	int new_size = 0;
	int maxi_connex_size_alt = 0;
	int new_size_alt = 0;
	bool visited[CELL_NUMBER] = {false};

	for (int i = 0; i < CELL_NUMBER; i++) {
		if (!visited[i]) {
			if (board -> cell_tab[i] -> level -> cell_type == cell_type) {
				new_size = get_connex_size(visited, board -> cell_tab[i], cell_type);
				new_size_alt = get_connex_size_with_altitude(visited, board -> cell_tab[i], cell_type);
				if (new_size > maxi_connex_size) {
					maxi_connex_size = new_size;
					maxi_connex_size_alt = new_size_alt;
				}
				else if (new_size == maxi_connex_size && new_size_alt > maxi_connex_size_alt) {
					maxi_connex_size_alt = new_size_alt;
				}
			}
			else {
				visited[i] = true;
			}
		}
	}

	return maxi_connex_size_alt;
}

void calculate_score_from_table(board_t * board) {
	board -> score = 0;
	
	board -> score += board -> table -> blue_mult * board -> table -> blue_nb_alt;
	board -> score += board -> table -> yellow_mult * board -> table -> yellow_nb_alt;
	board -> score += board -> table -> red_mult * board -> table -> red_nb_alt;
	board -> score += board -> table -> purple_mult * board -> table -> purple_nb_alt;
	board -> score += board -> table -> green_mult * board -> table -> green_nb_alt;
}

void update_scoring_table(board_t * board, tile_t * tile, int operation) {

	for (int i = 0; i < 6; i++) {
		switch (tile -> cell_types[i]) {
			case EMPTY:
				fprintf(stderr, "Tile should not contain EMPTY value %d\n", i);
			break;
			// blue
			case HOUSE_BLUE:
				if (operation == -1) {
					board -> table -> blue_nb_alt = 0;
				}
				else {
					board -> table -> blue_nb_alt = maximum_connex_size_with_altitude(board, HOUSE_BLUE);
				}
			break;
			// red
			case BARRAK_RED:
				if (cell_in_periphery(tile -> cell_tab[i])) {
					board -> table -> red_nb_alt += tile -> cell_tab[i] -> altitude * operation;
				}
			break;
			// yellow
			case MARKET_YELLOW:
				if (cell_isolated(tile -> cell_tab[i], tile -> cell_types[i])) {
					board -> table -> yellow_nb_alt += tile -> cell_tab[i] -> altitude * operation;
				}
			break;
			// purple
			case TEMPLE_PURPLE:
				if (cell_circled(tile -> cell_tab[i])) {
					board -> table -> purple_nb_alt += tile -> cell_tab[i] -> altitude * operation;
				}
			break;
			// green
			case PARK_GREEN:
				board -> table -> green_nb_alt += tile -> cell_tab[i] -> altitude * operation;
			break;
			// grey
			case QUARRY_GRAY:
				if (operation == -1) {
					board -> rocks++;
				}
			break;
			// blue
			case BLUE_PLACE:
				board -> table -> blue_mult++;
			break;
			// yellow
			case YELLOW_PLACE:
				board -> table -> yellow_mult++;
			break;
			// red
			case RED_PLACE:
				board -> table -> red_mult++;
			break;
			// purple
			case PURPLE_PLACE:
				board -> table -> purple_mult++;
			break;
			// green
			case GREEN_PLACE:
				board -> table -> green_mult++;
			break;
		}
	}
	calculate_score_from_table(board);
}

void remove_tile_from_board(board_t * board, tile_t * tile) {
	update_scoring_table(board, tile, -1);
	undo_without_null_tile(tile);
	update_scoring_table(board, tile, 1);
	for (int i = 0; i < 3; i++) {
		tile -> cell_tab[i] = NULL;
	}
}

void add_tile_to_board(board_t * board, tile_t * tile) {
	update_scoring_table(board, tile, -1);
	add_tile(tile);
	update_scoring_table(board, tile, 1);
}

linked_plays_t * gen_tiles(cell_t ** cell_tab, tile_t * tile) {

	play_t * list = malloc(sizeof(play_t));
	list -> tile = NULL;
	list -> next = NULL;

	linked_plays_t * linked_plays = malloc(sizeof(linked_plays_t));
	linked_plays -> size = 0;
	linked_plays -> play = list;

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
				linked_plays -> size++;
			}
			else {
				free(new_tile);
				free(play);
			}
		}
	}

	return linked_plays;
}

linked_plays_t * fusion_linked_plays(linked_plays_t * linked_plays_1, linked_plays_t * linked_plays_2) {
	
	play_t * cours = linked_plays_1 -> play;

	if (cours == NULL) {
		free(linked_plays_1);
		return linked_plays_2;
	}

	while (cours -> next != NULL) {
		cours = cours -> next;
	}
	
	cours -> next = linked_plays_2 -> play;
	linked_plays_1 -> size += linked_plays_2 -> size;

	free(linked_plays_2);
	return linked_plays_1;
}

linked_plays_t * gen_tiles_from_game(game_t * game, bool is_bot) {
	
	if (is_bot) {
		return fusion_linked_plays(gen_tiles(game -> bot -> cell_tab, game -> card_1), gen_tiles(game -> bot -> cell_tab, game -> card_2));
	}
	return fusion_linked_plays(gen_tiles(game -> player -> cell_tab, game -> card_1), gen_tiles(game -> player -> cell_tab, game -> card_2));
}

/*
 * Hashmaps functions
 */

