/* name : algos.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdio.h>
#include <stdlib.h>
#include "algos.h"
#include "utilities.h"

// /*


play_t *initialisation(game_t *game, hash_t *h) {

	play_t *play, *temp;
	int n;
	linked_plays_t *lp; 
	lp = gen_tiles(game->bot->cell_tab, game->card_1);

	n = lp->size;
	play = lp->play;
	
	lp = gen_tiles(game->bot->cell_tab, game->card_2);

	temp = play;
	while(temp->next != NULL) {
		temp = temp->next;
	}

	n += lp->size;
	temp->next = lp->play;

	// ADD TO HASHMAP

	return play;
}

play_t *selection(play_t *play) {
	
	play_t *temp = play;
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

simulation(play_t *play, int n) {
	apply_play(play);
	play_t *p;
	p = gen_plays()

}
// */


play_t *mcts(game_t *game) {
	hash_t *h = create_hash_map();
	play_t *p = initialisation(game, h);


}
