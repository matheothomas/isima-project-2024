/* name : algos.c
 * author : matheo thomas, domitille vale, eloi petit
 * date : 18-06-24
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "../include/algos.h"
#include "../include/utilities.h"
#include "../include/graphics.h"
#include "SDL2/SDL_timer.h"


int max(tree_t *tree, bool player) {
	int is_max = player ? 1 : -1;

	tree_t *temp = tree;
	int val_max = tree->value;

	while(temp->next_tree != NULL) {
		if (temp->value * is_max > val_max * is_max) {
			// tree_max = temp;
			val_max = temp->value;
		}
		temp = temp->next_tree;
	}

	return val_max;
}

play_t *max_play(tree_t *tree) {
	tree_t *tree_max = tree;
	tree_t *temp = tree;
	int val_max = tree->value;

	while(temp->next_tree != NULL) {
		if (temp->value > val_max) {
			tree_max = temp;
			val_max = temp->value;
		}
		temp = temp->next_tree;
	}

	return tree_max->play;
}

// play_t *choose_play(board_t *board, graphics_t *g, cell_t **cell_tab) {
play_t *choose_play(board_t *board) {
	bool player = true;
	tree_t *tree = gen_plays(board, 0, player);
	tree_t *temp = tree;

	while (temp->next_tree != NULL) {
		// display_board(g->board, g->white, g->white, g->window, g->renderer, cell_tab);
		// SDL_Delay(1000);
		// printf("play : %d\n", temp->play->cell_tab_length);
		if(validity_play(board, temp->play, player)) {
			temp->value = eval(apply_play(board, temp->play), 0, MAX_DEPTH, !player);
			undo_play(board, temp->play);
		}


		temp = temp->next_tree;
	}

	return max_play(tree);
}

board_t *apply_play(board_t *board, play_t *play) {
	// duplicates the balls
	for(int i = play->cell_tab_length - 1; i >= 0; i--) {
		// printf("test 3\n");

		if(play->cell_tab[i]->neighbourg[play->movement_direction] != NULL) {
			play->cell_tab[i]->neighbourg[play->movement_direction]->state = play->cell_tab[i]->state;
		}
	}

	// removes the old balls that have been duplicated
	if(play->movement_direction == play->cell_direction) {
		play->cell_tab[0]->state = EMPTY;
	} else {
		for(int i = 0; i < play->cell_tab_length; i++) {
			// play->cell_tab[i]->neighbourg[(play->movement_direction + 3) % 6]->state = EMPTY;
			play->cell_tab[i]->state = EMPTY;
		}
	}

	return board;
}

board_t *undo_play(board_t *board, play_t *play) {
	// duplicates the balls
	for(int i = 0; i < play->cell_tab_length; i++) {
		play->cell_tab[i]->state = play->buffer[i];
	}

	// removes the old balls that have been duplicated
	if(play->cell_direction == play->movement_direction) {
		if(play->cell_tab[play->cell_tab_length - 1]->neighbourg[play->cell_direction] != NULL) {
			play->cell_tab[play->cell_tab_length - 1]->neighbourg[play->cell_direction]->state = EMPTY;
		}
	} else {
		for(int i = 0; i < play->cell_tab_length; i++) {
			if(play->cell_tab[i]->neighbourg[play->movement_direction] != NULL) {
				play->cell_tab[i]->neighbourg[play->movement_direction]->state = EMPTY;
			}
		}
	}
	// printf("undo play\n");

	return board;
}

int eval(board_t *board, int depth, int max_depth, bool player) {
	// printf("print eval\n");
	int score = board->n_black - board->n_white;
	if (max_depth == depth || score == CELL_NUMBERS || score == -CELL_NUMBERS) {
		// printf("eval stop\n");
		return score;
	}
	tree_t *tree = gen_plays(board, depth, player);

	if(tree == NULL) {
		return CELL_NUMBERS;
	} else {


		tree_t *temp = tree;

		int i = 0;
		while(temp->next_tree != NULL) {
			if(validity_play(board, temp->play, player)) {
				temp->value = eval(apply_play(board, temp->play), depth + 1, max_depth, !player);

				undo_play(board, temp->play);

			}
			temp = temp->next_tree;
		}

		if(player) {
			return max(tree, player);
		} else {
			return max(tree, !player);
		}
	}
}
