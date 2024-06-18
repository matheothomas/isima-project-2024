#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/init.h"
#include "../include/algos.h"
#include "../include/utilities.h"


bool validity_play(board_t * board, play_t * play) {
	
	// Test if 


}

tree_t * create_tree(play_t * play, int value, int depth) {
	tree_t * tree = (tree_t *) malloc(sizeof(tree_t));
	tree -> value = value;
	tree -> depth = depth;
	tree -> next_tree = NULL;
	return tree;
}

void append_tree(tree_t * tree, play_t * play, int value, int depth) {
	tree_t *new_tree = create_tree(play, value, depth);
	tree_t * cours = tree;
	while (cours -> next_tree != NULL) {
		cours = cours -> next_tree;
	}
	cours -> next_tree = new_tree;
}

void traversal_rec(board_t * board, tree_t * tree, play_t * play, cell_t * cell, bool * visited, int cell_direction, bool player) {

	switch (cell -> state) {
		case EMPTY:
			if (play == NULL) {
				for (int i = 0; i < 6; i++) {
					traversal_rec(board, tree, NULL, (cell -> neighbourg)[i], visited, i, player);
				}
			}
			else {
				if (validity_play(board, play)) {
					append_tree(tree, play, 0, tree -> depth);
				}
			}
		break;

		case WHITE:
			if (play == NULL && visited[cell -> id] == false) {
				visited[cell -> id] = true;
				for (int i = 0; i < 6; i++) {
					for (int j = 0; j < 6; j++) {
						play_t new_play;
						new_play.cell_tab_length = 1;
						new_play.movement_direction = j;
						new_play.cell_direction = i;
						new_play.cell_tab[0] = cell;
						traversal_rec(board, tree, &new_play, cell -> neighbourg[i], visited, i, player);
					}
				}
			}
			else if (play -> cell_tab_length < 3) {
				play -> cell_tab[play -> cell_tab_length] = cell;
				play -> cell_tab_length ++;
				if (validity_play(board, play)) {
					append_tree(tree, play, 0, tree -> depth);
				}
				traversal_rec(board, tree, play, cell -> neighbourg[play -> cell_direction], visited, play -> cell_direction, player);
			}
		break;

		case BLACK:
			if (play != NULL && play -> cell_tab_length < 5) {
				play -> cell_tab[play -> cell_tab_length] = cell;
				play -> cell_tab_length ++;
				if (validity_play(board, play)) {
					append_tree(tree, play, 0, tree -> depth);
				}
				traversal_rec(board, tree, play, cell -> neighbourg[play -> cell_direction], visited, play -> cell_direction, player);
			}
		break;
	}

}

tree_t * gen_plays(board_t * board, int depth, bool player) {


	// Player = 1 if bot is the player else 0
	bool visited[CELL_NUMBER] = {false};
	tree_t * tree = create_tree(NULL, 0, depth); //tête de liste
	traversal_rec(board, tree, NULL, board -> cell, visited, 0, player);
	



	return tree -> next_tree;
}
