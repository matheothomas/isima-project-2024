/* name : algos.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "algos.h"
#include "init.h"
#include "utilities.h"


play_t *initialisation(game_t *game, hash_t **h) {

	linked_plays_t *lp = gen_tiles_from_game(game, true);
	hash_map_add(h, game->bot, lp);

	return lp->play;
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

play_t *get_random_tile(linked_plays_t *lp) {
	int play_i = rand() % lp->size;
	play_t *p = lp->play;

	while(play_i > 0) {
		p = p->next;

		play_i--;
	}
	return p;
}

int simulation(play_t *play, hash_t **h, game_t *game, bool is_bot, bool is_last_node) {
	// if(is_game_finished) {
		// return score;
	// }

	if(!is_last_node) {
		board_t *b = is_bot ? game->bot : game->player;

		uint32_t hash_value = hash_board(b);

		hash_t *hash_cell = h[hash_value];
		
		while (hash_cell != NULL && hash_cell->hashed_board != hash_value) {
			hash_cell = hash_cell->next;
		}

		if (hash_cell != NULL) {		// the node has already been explored before
			play_t *new_play = get_random_tile(hash_cell->plays);

			add_tile(play->tile);
			play->n_coup ++;
			int temp_score = simulation(new_play, h, game, !is_bot, is_last_node);
			play->gain_coup += temp_score;
			undo_tile(play->tile);

			// UNDO THE DECK
			return temp_score;

		} else {						// first time exploring this node, adding initialisation 
			linked_plays_t *lp = is_bot ? gen_tiles_from_game(game, is_bot) : gen_tiles_from_game(game, !is_bot);
			is_bot ? hash_map_add(h, game->bot, lp) : hash_map_add(h, game->player, lp);

			play_t *new_play = get_random_tile(lp);

			add_tile(play->tile);
			play->n_coup ++;
			int temp_score = simulation(new_play, h, game, !is_bot, is_last_node);
			play->gain_coup += temp_score;
			undo_tile(play->tile);

			// UNDO THE DECK
			return temp_score;
		}

	} else {							// random recursion until end game
		linked_plays_t *lp = is_bot ? gen_tiles_from_game(game, is_bot) : gen_tiles_from_game(game, !is_bot);
		// is_bot ? hash_map_add(h, game->bot, lp) : hash_map_add(h, game->player, lp);

		play_t *new_play = get_random_tile(lp);

		add_tile(play->tile);
		// play->n_coup ++;
		int temp_score = simulation(new_play, h, game, !is_bot, is_last_node);
		// play->gain_coup += temp_score;
		undo_tile(play->tile);

		// UNDO THE DECK
		return temp_score;
	}
}

play_t *mcts(game_t *game) {
	srand(time(0)); // TO BE PUT IN MAIN
	hash_t **h = create_hash_map();
	play_t *p = initialisation(game, h);
	play_t *p2;

	time_t t0 = time(0);
	time_t t1 = time(0);
	while(difftime(t1, t0) < 10) {
		p2 = selection(p);

		simulation(p2, h, game, 0, 0);




		t1 = time(0);
	}
	// TODO : RETURN THE BEST PLAY BY CALCULATING THE AVERAGE GAIN

}
