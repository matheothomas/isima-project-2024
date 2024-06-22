/* name : algos.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 18-06-24
 */
#define _GNU_SOURCE
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "algos.h"
#include "graphics.h"
#include "init.h"
#include "utilities.h"


// int play_count = 0;
// int undo_count = 0;


void init_play(play_t *play) {
	play->cell_direction = 0;
	play->cell_tab_length = 0;
	play->movement_direction = 0;
	for(int i = 0; i < 5; i++) {
		play->buffer[i] = 0;
		play->cell_tab[i] = NULL;
	}
}

play_t *create_play() {
	play_t *play = malloc(sizeof(play_t));
	init_play(play);
	return play;
}

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

	free_tree(tree);
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

	printf("max play value : %d\n", tree_max->value);
	return tree_max->play;
}

int basic_heuristic(cell_t **cell_tab) {
	// printf("basic_heuristic\n");
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

// int val_abs(int n) {
	// return n > 0 ? n : -n;
// }

int center_heuristic(cell_t **cell_tab, bool player) {
	int score = basic_heuristic(cell_tab);
	int nb_w = 0;
	// int nb_b = 0;
	int sum_w = 0;
	// int sum_b = 0;
	for(int i = 0; i < CELL_NUMBER; i++) {
		if(cell_tab[i]->state == WHITE) {
			nb_w++;
			sum_w += i;
		}
		// if(cell_tab[i]->state == BLACK) {
			// nb_b++;
			// sum_b += i;
		// }
	}
	if(player) {
		// return 1000 + score * 100 - val_abs(((float)sum_w / nb_w - 30) * 10);
		return 1000 + score * 100 - (((float)sum_w / nb_w) - 30) * 10;
	} else {
		// return 1000 + score * 100 + val_abs(((float)sum_w / nb_w - 30) * 10);
		return 1000 + score * 100 + (((float)sum_w / nb_w) - 30) * 10;
	}
}

int * get_temp_address(tree_t ** current_tree, pthread_mutex_t * tree_mutex) {
	int * returned_value_address = NULL;
	if (pthread_mutex_lock(tree_mutex)) {
		fprintf(stderr, "Error locking tree_mutex in get_temp_address\n");
	}
	if (*current_tree != NULL) {
		returned_value_address = &((*current_tree) -> value);
		*current_tree = (*current_tree) -> next_tree;
	}

	if (pthread_mutex_unlock(tree_mutex)) {
		fprintf(stderr, "Error locking tree_mutex in get_temp_address\n");
	}

	return returned_value_address;
}

void * create_thread(void * args) {
	args_t * vals = args;
	int * temp_value = NULL;
	while ((temp_value = get_temp_address(vals -> temp, vals -> tree_mutex)) != NULL) {
		eval_thread(vals -> board, vals -> cell_tab, temp_value, vals -> depth, vals -> max_depth, vals -> player, vals -> alpha, vals -> beta);
		printf("Searching\n");
	}
	pthread_exit(NULL);
}


board_t * copy_board(cell_t ** cell_tab) {
	board_t * new_board = create_clean_board();
	cell_t ** new_cell_tab = create_table(*new_board);

	for (int i = 0; i < CELL_NUMBER; i++) {
		new_cell_tab[i] -> state = cell_tab[i] -> state;
		new_cell_tab[i] -> selection = cell_tab[i] -> selection;
	}

	return new_board;
}

cell_t ** copy_cell_tab(board_t * board) {
	cell_t ** new_cell_tab = create_table(*board);

	return new_cell_tab;
}

// play_t *choose_play(board_t *board, graphics_t *g, cell_t **cell_tab) {
play_t *choose_play(board_t *board, cell_t **cell_tab, bool player) {
	// printf("choose_play\n");
	// play_count = 0;
	// undo_count = 0;
	tree_t *tree = gen_plays(board, 0, player);
	tree_t *temp = tree;

	if(tree == NULL) {
		// printf("something is null\n");
		return NULL;
	}

	// int num_threads = 4;
	// pthread_t threads[num_threads];
	// args_t arguments[num_threads];

	// for (int i = 0; i < num_threads; i++) {
		// arguments[i].board = copy_board(cell_tab);
		// arguments[i].cell_tab = copy_cell_tab(board);
		// arguments[i].temp_value = NULL;
		// arguments[i].depth = 0;
		// arguments[i].max_depth = MAX_DEPTH;
		// arguments[i].player = !player;
		// arguments[i].alpha = INT_MIN;
		// arguments[i].beta = INT_MAX;
	// }

	// int active_threads = 0;
	while (temp->next_tree != NULL) {

	pthread_mutex_t tree_mutex = PTHREAD_MUTEX_INITIALIZER;

	int num_threads = 8;
	pthread_t threads[num_threads];
	args_t arguments[num_threads];

	for (int i = 0; i < num_threads; i++) {
		arguments[i].board = copy_board(cell_tab);
		arguments[i].cell_tab = copy_cell_tab(board);
		arguments[i].temp = &temp;
		arguments[i].tree_mutex = &tree_mutex;
		arguments[i].depth = 0;
		arguments[i].max_depth = MAX_DEPTH;
		arguments[i].player = !player;
		arguments[i].alpha = INT_MIN;
		arguments[i].beta = INT_MAX;
	}
	
	for (int i = 0; i < num_threads; i++) {
		if(validity_play(temp->play, player)) {
			// if (active_threads < num_threads) {
				// arguments[active_threads].temp_value = &temp -> value;
				// int flag = pthread_create(&threads[active_threads], NULL, create_thread, (void *) &arguments[active_threads]);
				// if (flag) {
					// fprintf(stderr, "Thread failed to initialize: %d\n", flag);
				// }
				// active_threads++;
			// }
			// else {
				temp->value = eval(apply_play(board, temp->play), cell_tab, 0, MAX_DEPTH, !player, INT_MIN, INT_MAX);
				undo_play(board, temp->play);
			// }
			printf("temp->value : %d\n", temp->value);
			int flag = pthread_create(&threads[i], NULL, create_thread, (void *) &arguments[i]);
			if (flag) {
				fprintf(stderr, "Thread failed to initialize: %d\n", flag);
			}
			printf("temp->value : %d\n", temp->value);
		}

		temp = temp->next_tree;
	}

	// Wait for all the threads to finish
	for (int i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	// printf("play_count : %d\n", play_count);
	// printf("undo_count : %d\n", undo_count);
	return max_play(tree);
}

board_t *apply_play(board_t *board, play_t *play) {
	// printf("apply_play\n");
	// play_count++;

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
	// undo_count++;

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

void eval_thread(board_t * board, cell_t ** cell_tab, int * temp_value, int depth, int max_depth, bool player, int alpha, int beta) {
	// printf("eval\n");
	int score;

	// score = basic_heuristic(cell_tab);
	score = center_heuristic(cell_tab, player);
	
	if (max_depth == depth || score == CELL_NUMBER/2 || score == -CELL_NUMBER/2) {
		*temp_value = score;
	}
	tree_t *tree = gen_plays(board, depth, player);

	if(tree == NULL) {
		*temp_value = score;
		
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
			*temp_value = max(tree, !player);
		} else {
			*temp_value = max(tree, player);
		}
	}
}

int eval(board_t *board, cell_t **cell_tab, int depth, int max_depth, bool player, int alpha, int beta) {
	// printf("eval\n");
	int score;

	// score = basic_heuristic(cell_tab);
	score = center_heuristic(cell_tab, player);

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
			return max(tree, player);
		} else {
			return max(tree, !player);
		}
	}
}
