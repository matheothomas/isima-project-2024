/* name : main.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 18-06-24
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "algos.h"
#include "init.h"
#include "utilities.h"


void print_play(play_t *play) {
	if(validity_play(play, 1)) {
		printf("ids: ");
		for(int i = 0; i < play->cell_tab_length; i++) {
			printf("%d ", play->cell_tab[i]->id);
		}
		printf("\n buffer : ");
		for(int i = 0; i < play->cell_tab_length; i++) {
			printf("%d ", play->buffer[i]);
		}
		printf("\ncell_tab_length : %d\n movement_direction : %d\n cell_direction %d\n", 
		 play->cell_tab_length, 
		 play->movement_direction, 
		 play->cell_direction);
		printf("validity : %d\n", validity_play(play, 1));

	}
	printf("\n buffer : ");
	for(int i = 0; i < play->cell_tab_length; i++) {
		printf("%d ", play->buffer[i]);
	}
	printf("\ncell_tab_length : %d\n movement_direction : %d\n cell_direction %d\n", play->cell_tab_length, play->movement_direction, play->cell_direction);
	printf("validity : %d\n", validity_play(play, 1));

}

bool is_duplicate(play_t * play1, play_t * play2) {
	// printf("is_duplicate\n");

	if (play1 != NULL && play2 != NULL) {

		// Can be a duplicate only if both have the same number of cells 
		// and movement_direction are equal
		if (play1 -> cell_tab_length == play2 -> cell_tab_length &&
			play1 -> movement_direction == play2 -> movement_direction) {
			int length = play1 -> cell_tab_length;

			// If cell_direction and movement_direction are not colinear
			if (play1 -> cell_direction != play1 -> movement_direction && 
				play1 -> cell_direction != (play1 -> movement_direction + 3) % 6) {

				// Can be a duplicate only if extremums of cell_tabs are equals (invered or not)
				if ((play1 -> cell_tab[0] -> id == play2 -> cell_tab[length - 1] -> id &&
					play2 -> cell_tab[0] -> id == play1 -> cell_tab[length - 1] -> id) || 
					(play1 -> cell_tab[0] -> id == play2 -> cell_tab[0] -> id && 
					play2 -> cell_tab[length - 1] -> id == play1 -> cell_tab[length - 1] -> id))
					return true;
			}
			else {
				// We only need to check the buffer and ids in that case 
				// (both cell_directions are positively colinear)
				for (int i = 0; i < length; i++) {
					if (play1 -> cell_tab[i] -> id != play2 -> cell_tab[i] -> id)
						return false;
					if (play1 -> buffer[i] != play2 -> buffer[i])
						return false;
				}
				return true;
			}
		}
	}
	return false;
}

bool validity_play(play_t * play, bool player) {

	if (play == NULL) {
		return false;
	}
	printf("platy  %d\n", play -> cell_tab[0] -> id);
	// cell_direction is meaningless when play is of size 1 
	// and creates a lot of duplicates that we can remove early
	if (play -> cell_tab_length == 1 && play -> cell_direction != play -> movement_direction) {
		return false;
	}

	state_e switch_player_color[2] = {BLACK, WHITE};

	// Check if the play line is well ordered
	bool changed_to_non_player_color = false;
	cell_t * cell = play -> cell_tab[0];
	int i = 0;
	while (cell != NULL && i++ < 6 && cell -> state != EMPTY) {
		if (cell -> state == switch_player_color[!player]) {
			changed_to_non_player_color = true;
		}
		// If we find a cell belonging to player after a non player cell then the play is invalid
		else if (cell -> state == switch_player_color[player] && changed_to_non_player_color) {
			puts("a");
			return false;
		}
		cell = cell -> neighbor[play -> cell_direction];
	}

	printf("value 2 %d\n", player);
	// Check if movemement is valid when movement_direction and cell_direction are positively colinear
	if (play -> cell_direction == play -> movement_direction) {
		cell_t * cours = play -> cell_tab[0];
		int player_cells = 0;
		int total_cells = 0;
		
		// We go further than the play length because of case 3 player cells
		// then 3 non player cells -> last non player cell is possibly not accounted for in cell_tab
		while (cours != NULL && total_cells < 6 && cours -> state != EMPTY) {	
			player_cells += (switch_player_color[player] == cours -> state) ? 1 : 0;
			total_cells++;
			cours = cours -> neighbor[play -> cell_direction];
		}
		//printf("player_cells : %d total_cells : %d\n", player_cells, total_cells);
		if (2 * player_cells <= total_cells) {
			//printf("player_cells false\n");
			puts("b");
			return false;
		}

		// Handle cells being thrown off board
		for (int j = 0; j < play -> cell_tab_length; j++) {
			if (play -> cell_tab[j] -> neighbor[play -> movement_direction] == NULL) {

				if(play -> cell_tab[j] -> state == switch_player_color[player]) {
					puts("c");
					return false;
				}
			}
		}
		if (play -> cell_tab[play -> cell_tab_length - 1] -> neighbor[play -> movement_direction] != NULL && 
			play -> cell_tab[play -> cell_tab_length - 1] -> neighbor[play -> movement_direction]->state == 
			switch_player_color[player]) {
			puts("la mort");
			return false;
		}
	}
	// Check if movement is valid otherwise
	else {
		for (int k = 0; k < play -> cell_tab_length; k++) {
			if (play -> cell_tab[k] -> neighbor[play -> movement_direction] == NULL) {
				return false;
			}
			if (play -> cell_tab[k] -> neighbor[play -> movement_direction] -> state != EMPTY) {
				//printf("c\n");
				puts("e");
				return false;
			}
		}
	}

	if (play->cell_tab_length > 1 && 
		play->cell_tab[0]->neighbor[play->cell_direction] != play->cell_tab[1]) {
		return false;
	}
	return true;
}

void free_tree(tree_t * tree) {
	
	while (tree != NULL) {
		tree_t * previous = tree;
		tree = tree -> next_tree;
		free(previous -> play);
		free(previous);
	}
}

tree_t * create_tree(play_t * play, int value, int depth) {
	// printf("create_tree\n");

	tree_t * tree = malloc(sizeof(tree_t));
	tree -> play = play;
	tree -> value = value;
	tree -> depth = depth;
	tree -> next_tree = NULL;

	return tree;
}

void append_tree(tree_t * tree, play_t * play, int value, int depth, bool player) {
	printf("player1 %d\n", player);
	if(validity_play(play, player)) {
		tree_t *new_tree = create_tree(play, value, depth);
		tree_t * cours = tree;

		while (cours -> next_tree != NULL) {
			if (is_duplicate(play, cours -> play)) {
				return;
			}
			cours = cours -> next_tree;
		}
		if (!is_duplicate(cours -> play, play)) {
			cours -> next_tree = new_tree;
		}
	}
}

void fill_play_buffer(play_t * play) {
	// printf("fill_play_buffer\n");

	if (play -> cell_tab_length < 5) {
		play -> cell_tab[play -> cell_tab_length] = NULL;
	}

	int i = 0;
	while (i < play -> cell_tab_length) {
		play -> buffer[i] = play -> cell_tab[i] -> state;
		i++;
	}
}

void cell_belongs_to_player(tree_t * tree, play_t * play, cell_t * cell, bool * visited, bool player) {

	if (play == NULL && visited[cell -> id] == true) {
		return;
	}
	else if (visited[cell -> id] == false) {
		visited[cell -> id] = true;

		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				play_t * new_play = malloc(sizeof(play_t));
				new_play -> cell_tab_length = 1;
				new_play -> movement_direction = j;
				new_play -> cell_direction = i;
				new_play -> cell_tab[0] = cell;
				new_play -> cell_tab[1] = NULL;
				new_play -> cell_tab[2] = NULL;
				new_play -> cell_tab[3] = NULL;
				new_play -> cell_tab[4] = NULL;
				for (int k = 0; k < 5; k++) {
					new_play -> buffer[k] = 0;
				}
				traversal_rec(board, tree, new_play, cell -> neighbor[i], visited, player);
				//printf("direction %d\n", new_play -> movement_direction);

				traversal_rec(board, tree, new_play, cell -> neighbor[i], visited, player);
			}
		}
	}
	else if (play -> cell_tab_length < 3) {

		play -> cell_tab[play -> cell_tab_length] = cell;
		play -> cell_tab_length ++;
		printf("player1 %d\n", player);
		if (validity_play(play, player)) {
			fill_play_buffer(play);
			append_tree(tree, play, 0, tree -> depth);
		}	
		traversal_rec(tree, play, cell -> neighbor[play -> cell_direction], visited, player);
	}

}

void cell_does_not_belongs_to_player(tree_t * tree, play_t * play, cell_t * cell, bool * visited, bool player) {

	if (play != NULL) {
		// If movement direction is not colinear to cell direction then we should
		// not add the cell of the other player
		if (play -> cell_tab_length < 5) {
			if (play -> cell_direction == play -> movement_direction || 
				play -> cell_direction == (play -> movement_direction + 3) % 6) {

				play -> cell_tab[play -> cell_tab_length] = cell;
				play -> cell_tab_length ++;
				printf("player1 %d\n", player);
				if (validity_play(play, player)) {
					fill_play_buffer(play);
					append_tree(tree, play, 0, tree -> depth, player);
				}
				traversal_rec(tree, play, cell -> neighbor[play -> cell_direction], visited, player);
			}
		}
	}
	else {
		if (!visited[cell -> id]) {
			visited[cell -> id] = true;
			for (int i = 0; i < 6; i++) {
				traversal_rec(tree, NULL, cell -> neighbor[i], visited, player);
			}
		}
	}
}

void traversal_rec(tree_t * tree, play_t * play, cell_t * cell, bool * visited, bool player) {

	if (cell == NULL) { return; }

	switch (cell -> state) {
		case EMPTY:
			if (!visited[cell -> id]) {
				visited[cell -> id] = true;
				if (play == NULL) {
					for (int i = 0; i < 6; i++) {
						traversal_rec(tree, NULL, cell -> neighbor[i], visited, player);
					}
				}
				else {
					printf("player %d\n", player);
					if (validity_play(play, player)) {
						fill_play_buffer(play);
						if(play->cell_tab[0]->id == 5) {
							// printf("putputput\n");
						}
						append_tree(tree, play, 0, tree -> depth, player);
						for (int i = 0; i < 6; i++) {
							traversal_rec(tree, NULL, cell -> neighbor[i], visited, player);
						}
					}
				}
			}
			else {
				printf("player %d\n", player);
				if (validity_play(play, player)) {
					fill_play_buffer(play);
					append_tree(tree, play, 0, tree -> depth, player);
				}
			}
		break;

		case WHITE:
			(player) ? 
				cell_belongs_to_player(tree, play, cell, visited, player) :
				cell_does_not_belongs_to_player(tree, play, cell, visited, player);
		break;

		case BLACK:
			(!player) ?
				cell_belongs_to_player(tree, play, cell, visited, player) : 
				cell_does_not_belongs_to_player(tree, play, cell, visited, player);

		break;
	}
}

tree_t * gen_plays(board_t * board, int depth, bool player) {
	// printf("gen_plays\n");

	// Player = 1 if bot is the player else 0
	bool * visited = malloc(sizeof(bool) * CELL_NUMBER);
	for (int i = 0; i < CELL_NUMBER; i++) {
		visited[i] = false;
	}
	tree_t * tree = create_tree(NULL, 0, depth); //tête de liste
	traversal_rec(tree, NULL, board -> cell, visited, player);

	return tree -> next_tree;
}
