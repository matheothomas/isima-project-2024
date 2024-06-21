/* name : algos.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 18-06-24
 */

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "algos.h"
#include "graphics.h"
#include "init.h"
#include "utilities.h"


// extern bool global_player;

int play_count = 0;
int undo_count = 0;

int max_value(int a, int b) {
	// printf("max_value\n");
	return a < b ? b : a;
}

int min_value(int a, int b) {
	// printf("min_value\n");
	return a < b ? a : b;
}

int max(tree_t *tree, bool player) {
	// printf("max\n");
	int is_max = player ? 1 : -1;

	tree_t *temp = tree;
	int val_max = tree->value;

	while(temp->next_tree != NULL) {
		if (temp->value * is_max > val_max * is_max) {
			val_max = temp->value;
		}
		temp = temp->next_tree;
	}

	return val_max;
}

play_t *max_play(tree_t *tree) {
	// printf("max_play\n");
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

	// printf("max play value : %d\n", tree_max->value);
	return tree_max->play;
}

int basic_heuristic(cell_t **cell_tab, bool player) {
	// printf("basic_heuristic\n");
	(void)player;
	int score;
	int nb_white = 0;
	int nb_black = 0;
	for(int i = 0; i < CELL_NUMBER; i++) {
		if(cell_tab[i]->state == WHITE) {
			nb_white++;
		}
		if(cell_tab[i]->state == BLACK) {
			nb_black++;
		}
	}
	score = nb_white - nb_black;

	return score;
}

int center_heuristic(cell_t **cell_tab, bool player) {
	int score = basic_heuristic(cell_tab, player);
	int white_count = 0;
	int black_count = 0;
	int white_sum = 0;
	int black_sum = 0;

	for(int i = 0; i < CELL_NUMBER; i++) {
		if(cell_tab[i]->state == WHITE) {
			white_count++;
			white_sum += i;
		} else if (cell_tab[i]->state == BLACK) {
			black_count++;
			black_sum += i;
		}
	}

	int white_avg = white_sum / white_count;
	int black_avg = black_sum / black_count;

	if(player) {
		return score * 10000 + (float)1000/(white_avg - CELL_NUMBER / 2);
	} else {
		return -(score * 10000 + (float)1000/(black_avg - CELL_NUMBER / 2));
	}
}

// play_t *choose_play(board_t *board, graphics_t *g, cell_t **cell_tab) {
play_t *choose_play(board_t *board, cell_t **cell_tab, bool player) {
	// global_player = player;
	// printf("choose_play\n");
	play_count = 0;
	undo_count = 0;
	tree_t *tree = gen_plays(board, 0, player);
	tree_t *temp = tree;

	if(tree == NULL) {
		// printf("something is null\n");
		return NULL;
	}
	while (temp->next_tree != NULL) {
		// display_board(g->board, g->white, g->white, g->window, g->renderer, cell_tab);
		// SDL_Delay(1000);
		// printf("play : %d\n", temp->play->cell_tab_length);
		if(validity_play(temp->play, player)) {
			temp->value = eval(apply_play(board, temp->play), cell_tab, 0, MAX_DEPTH, !player, INT_MIN, INT_MAX);
			// printf("temp->value : %d\n", temp->value);
			undo_play(board, temp->play);
		}

		temp = temp->next_tree;
	}

	printf("play_count : %d\n", play_count);
	printf("undo_count : %d\n", undo_count);
	return max_play(tree);
}

board_t *apply_play(board_t *board, play_t *play) {
	if(play == NULL) {
		printf("PLAY NULL\n");
		return board;
	}
	// printf("apply_play\n");
	play_count++;

	// duplicates the balls
	for(int i = play->cell_tab_length - 1; i >= 0; i--) {
		if(play->cell_tab[i]->neighbor[play->movement_direction] != NULL) {
			play->cell_tab[i]->neighbor[play->movement_direction]->state = play->cell_tab[i]->state;
		} else {
			if (play->cell_tab[i]->state == WHITE) {
				board->n_white -= 1;
			} else if (play->cell_tab[i]->state == BLACK) {
				board->n_black -= 1;
			}
		}
	}

	// removes the old balls that have been duplicated
	if(play->movement_direction == play->cell_direction) {
		play->cell_tab[0]->state = EMPTY;
	} else {
		for(int i = 0; i < play->cell_tab_length; i++) {
			play->cell_tab[i]->state = EMPTY;
		}
	}

	return board;
}

board_t *undo_play(board_t *board, play_t *play) {
	// printf("undo_play\n");
	undo_count++;

	// duplicates the balls
	if(play->cell_tab[play->cell_tab_length - 1]->neighbor[play->cell_direction] == NULL) {
		if (play->buffer[play->cell_tab_length - 1] == WHITE) {
			board->n_white += 1;
		} else if (play->buffer[play->cell_tab_length - 1] == BLACK) {
			board->n_black += 1;
		}
	}

	for(int i = 0; i < play->cell_tab_length; i++) {
		play->cell_tab[i]->state = play->buffer[i];
	}

	// removes the old balls that have been duplicated
	if(play->cell_direction == play->movement_direction) {
		if(play->cell_tab[play->cell_tab_length - 1]->neighbor[play->cell_direction] != NULL) {
			play->cell_tab[play->cell_tab_length - 1]->neighbor[play->cell_direction]->state = EMPTY;
		}
	} else {
		for(int i = 0; i < play->cell_tab_length; i++) {
			if(play->cell_tab[i]->neighbor[play->movement_direction] != NULL) {
				play->cell_tab[i]->neighbor[play->movement_direction]->state = EMPTY;
			}
		}
	}

	return board;
}

int eval(board_t *board, cell_t **cell_tab, int depth, int max_depth, bool player, int alpha, int beta) {
	// printf("eval\n");

	int score = basic_heuristic(cell_tab, player);
	// int score = center_heuristic(cell_tab, player);
	// if(score != 0) {
		// printf("score : %d\n", score);
	// }

	if (max_depth == depth || score == CELL_NUMBER/2 || score == -CELL_NUMBER/2) {
		return score;
	}
	tree_t *tree = gen_plays(board, depth, player);

	if(tree == NULL) {
		return score;
	} else {
		tree_t *temp = tree;

		while(temp->next_tree != NULL) {
			if(validity_play(temp->play, player)) {
				temp->value = eval(apply_play(board, temp->play), cell_tab, depth + 1, max_depth, !player, alpha, beta);
				undo_play(board, temp->play);

				if(player) {
					alpha = max_value(alpha, temp->value);
					if(beta <= alpha) {
						break;
					}
				} else {
					beta = min_value(beta, temp->value);
					if(beta <= alpha) {
						break;
					}
				}
			}
			temp = temp->next_tree;
		}

		if(player) {
			return max(tree, !player);
		} else {
			return max(tree, player);
		}
	}
}
