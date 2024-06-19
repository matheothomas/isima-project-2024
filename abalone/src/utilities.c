#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/init.h"
#include "../include/algos.h"
#include "../include/utilities.h"


bool validity_play(board_t * board, play_t * play, bool player) {

	state_e switch_player_color[2] = {BLACK, WHITE};

	// Check if the play line is well ordered
	bool changed_to_non_player_color = false;
	cell_t * cell = play -> cell_tab[0];
	int i = 0;
	while (cell != NULL && i++ < 6 && cell -> state != EMPTY) {
		// TODO raccourcir condition et consequence
		if (cell -> state == switch_player_color[!player]) {
			changed_to_non_player_color = true;
		}
		// If we find a cell belonging to player after a non player cell then the play is invalid
		else if (cell -> state == switch_player_color[player] && changed_to_non_player_color) {
			// puts("a");
			return false;
		}
		cell = cell -> neighbourg[play -> cell_direction];
	}

	// Check if movemement is valid when movement_direction and cell_direction are colinear
	if (play -> cell_direction == play -> movement_direction || (play -> cell_direction + 3) % 6 == play -> movement_direction) {
		// We go further than the play length because of case 3 player cells then 3 non player cells -> last non player cell not accounted for in play structure
		cell_t * cours = play -> cell_tab[0];
		int player_cells = 0;
		int total_cells = 0;
		while (cours != NULL && total_cells < 6 && cours -> state != EMPTY) {

			player_cells += (switch_player_color[player] == cours -> state) ? 1 : 0;
			total_cells++;
			cours = cours -> neighbourg[play -> cell_direction];
		}
		//printf("player_cells : %d total_cells : %d\n", player_cells, total_cells);
		if (2 * player_cells <= total_cells) {
			//printf("player_cells false\n");
			// puts("b");
			return false;
		}

		// Handle cells being thrown off board
		for (int j = 0; j < play -> cell_tab_length; j++) {
			if (play -> cell_tab[j] -> neighbourg[play -> movement_direction] == NULL) {

				if(play -> cell_tab[j] -> state == switch_player_color[player]) {
					// puts("c");
					return false;
				}
			}
		}
		if(play->cell_tab[play->cell_tab_length - 1]->neighbourg[play->movement_direction]->state == switch_player_color[player]) {
			// TODO YA UNE SEGFAULT QUAND ON ACCEDE AU VOISIN
			// puts("la mort");
			return false;
		}
	}
	// Check if movement is valid otherwise
	else {
		for (int k = 0; k < play -> cell_tab_length; k++) {
			if (play -> cell_tab[k] -> neighbourg[play -> movement_direction] == NULL) {
				// puts("d");
				return false;
			}
			if (play -> cell_tab[k] -> neighbourg[play -> movement_direction] -> state != EMPTY) {
				//printf("c\n");
				// puts("e");
				return false;
			}
		}
	}

	if (play->cell_tab_length > 1 && play->cell_tab[0]->neighbourg[play->cell_direction] != play->cell_tab[1]) {
		// puts("f");
		return false;
	}
	return true;
}

tree_t * create_tree(play_t * play, int value, int depth) {

	tree_t * tree = malloc(sizeof(tree_t));
	tree -> play = play;
	tree -> value = value;
	tree -> depth = depth;
	tree -> next_tree = NULL;

	return tree;
}

void append_tree(tree_t * tree, play_t * play, int value, int depth) {
	if(validity_play(NULL, play, 1)) {
		tree_t *new_tree = create_tree(play, value, depth);
		tree_t * cours = tree;

		while (cours -> next_tree != NULL) {
			cours = cours -> next_tree;
		}

		cours -> next_tree = new_tree;
	}
}

void fill_play_buffer(play_t * play) {

	if (play -> cell_tab_length < 5) {
		play -> cell_tab[play -> cell_tab_length] = NULL;
	}

	int i = 0;
	while (i < play -> cell_tab_length) {
		play -> buffer[i] = play -> cell_tab[i] -> state;
		i++;
	}
}

void cell_belongs_to_player(board_t * board, tree_t * tree, play_t * play, cell_t * cell, bool * visited, bool player) {

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
				//printf("direction %d\n", new_play -> movement_direction);

				traversal_rec(board, tree, new_play, cell -> neighbourg[i], visited, player);
			}
		}
	}
	else if (play -> cell_tab_length < 3) {

		play -> cell_tab[play -> cell_tab_length] = cell;
		play -> cell_tab_length ++;

		if (validity_play(board, play, player)) {
			fill_play_buffer(play);
			if(play->cell_tab[0]->id == 5) {
				// printf("oulala %d\n", play->cell_tab_length);
			}
			append_tree(tree, play, 0, tree -> depth);
		}

		traversal_rec(board, tree, play, cell -> neighbourg[play -> cell_direction], visited, player);
	}

}

void cell_does_not_belongs_to_player(board_t * board, tree_t * tree, play_t * play, cell_t * cell, bool * visited, bool player) {
	if (play != NULL && play -> cell_tab_length < 5) {
		// If momvement direction is not colinear to cell direction then we should not add the cell of the other player
		if (play -> cell_direction == play -> movement_direction || (play -> cell_direction + 3) % 6 == play -> movement_direction) {
			play -> cell_tab[play -> cell_tab_length] = cell;
			play -> cell_tab_length ++;

			if (validity_play(board, play, player)) {
				fill_play_buffer(play);
				if(play->cell_tab[0]->id == 5) {
					// printf("putput\n");
				}
				append_tree(tree, play, 0, tree -> depth);
			}

			traversal_rec(board, tree, play, cell -> neighbourg[play -> cell_direction], visited, player);
		}
	}
}

void traversal_rec(board_t * board, tree_t * tree, play_t * play, cell_t * cell, bool * visited, bool player) {

	if (cell == NULL) { return; }

	switch (cell -> state) {
		case EMPTY:
			if (!visited[cell -> id]) {
				visited[cell -> id] = true;
				if (play == NULL) {
					for (int i = 0; i < 6; i++) {
						traversal_rec(board, tree, NULL, cell -> neighbourg[i], visited, player);
					}
				}
				else {
					if (validity_play(board, play, player)) {
						fill_play_buffer(play);
						if(play->cell_tab[0]->id == 5) {
							// printf("putputput\n");
						}
						append_tree(tree, play, 0, tree -> depth);
						for (int i = 0; i < 6; i++) {
							traversal_rec(board, tree, NULL, cell -> neighbourg[i], visited, player);
						}
					}
				}
			}
		break;

		case WHITE:
			(player) ? cell_belongs_to_player(board, tree, play, cell, visited, player) : cell_does_not_belongs_to_player(board, tree, play, cell, visited, player);
		break;

		case BLACK:
			(!player) ? cell_belongs_to_player(board, tree, play, cell, visited, player) : cell_does_not_belongs_to_player(board, tree, play, cell, visited, player);

		break;
	}

}

tree_t * gen_plays(board_t * board, int depth, bool player) {

	// Player = 1 if bot is the player else 0
	bool visited[CELL_NUMBER] = {false};
	tree_t * tree = create_tree(NULL, 0, depth); //tête de liste
	// printf("%p\n", (board) -> cell);
	traversal_rec(board, tree, NULL, board -> cell, visited, player);

	return tree -> next_tree;
}
