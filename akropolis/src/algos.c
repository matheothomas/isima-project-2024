/* name : algos.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdio.h>
#include <stdlib.h>
#include "algos.h"

tree_t *initialisation(game_t *game) {
	tree_t *tree = malloc(sizeof(tree_t));
	play_t *p1 = malloc(sizeof(play_t));
	play_t *p2 = malloc(sizeof(play_t));
	
	p1->tile = game->card_1;
	p1->n_coup = 0;
	p1->gain_coup = 0;
	p1->next = p2;
	
	p2->tile = game->card_2;
	p2->n_coup = 0;
	p2->gain_coup = 0;
	p2->next = NULL;

	tree->play = p1;
	tree->next = NULL;

	return tree;
}

play_t *selection(tree_t *tree) {
	play_t *temp = tree->play;
	while(temp != NULL) {
		if (temp->n_coup == 0) {
			break;
		}
		temp = temp->next;
	}
	if (temp == NULL) {
		// UCB
	}

	return temp;
}

tree_t *development(play_t *play) {
	tree_t *tree = malloc(sizeof(tree_t));

}
