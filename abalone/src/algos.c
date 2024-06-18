/* name : algos.c
 * author : matheo thomas
 * date : 18-06-24
 */

#include <stdbool.h>
#include "../include/algos.h"


play_t *choose_play(tree_t *tree, bool is_bot) {
	int is_max = is_bot ? 1 : -1;

	int i_max = 0;
	int val_max = tree->tree_tab[0].value;

	for(int i = 1; i < tree->tab_length; i++) {
		if (tree->tree_tab[i].value * is_max > val_max * i_max) {
			i_max = i;
			val_max = tree->tree_tab[i].value;
		}
	}
	return tree->tree_tab[i_max].play;
}



tree_t *apply_play(tree_t *tree, board_t *board, play_t *play, bool is_bot) {
	for(int i = play->cell_tab_length-1; i > 0; i--) {
		play->cell_tab[i]->neighbourg[play->direction]->state = play->cell_tab[i]->state;
	}
	play->cell_tab[0]->state = EMPTY;
	return tree;
}

tree_t *eval(tree_t *tree, board_t *board, int max_depth, bool is_bot) {
	if (max_depth == tree->depth) {
		tree->value = board->n_black - board->n_white;
		return tree;
	}

	if(is_bot) {
		return eval(apply_play(tree, board, choose_play(tree, is_bot), is_bot), board, max_depth+1, false);
	} else {
		return eval(apply_play(tree, board, choose_play(tree, is_bot), is_bot), board, max_depth+1, true);
	}
}
