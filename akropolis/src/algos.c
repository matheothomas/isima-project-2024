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

float G(play_t *play) {
	return play->gain_coup / (float)play->n_coup;
}

float interest(play_t *play, float c, int n) {
	return G(play) + c * sqrtf(logf(n) / play->n_coup);
}

play_t *ucb(play_t *play, float c, int n) {
	float temp_interest = 0, interest_max = 0;
	play_t *max_play;


	while (play != NULL) {
		temp_interest = interest(play, c, n);
		if (temp_interest > interest_max) {
			interest_max = temp_interest;
			max_play = play;
		}

		play = play->next;
	}

	return max_play;
}

play_t *selection(play_t *play, float c, int n) {

	play_t *temp = play;
	while(temp != NULL) {
		if (temp->n_coup == 0) {
			break;
		}
		temp = temp->next;
	}
	if (temp == NULL) {
		// UCB
		temp = ucb(play, c, n);
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

bool is_game_finished(game_t *game) {
	return game->deck->n == DECK_NUMBER ? true : false;
}

int calculate_score(game_t *game, bool is_bot) {
	return is_bot ? game->bot->score - game->player->score : game->player->score - game->bot->score ;
}

int simulation(play_t *play, hash_t **h, game_t *game, bool is_bot, bool is_last_node) {
	if(is_game_finished(game)) {
		return calculate_score(game, is_bot);
	}

	if(!is_last_node) {
		board_t *b = is_bot ? game->bot : game->player;

		uint32_t hash_value = hash_board(b);

		hash_t *hash_cell = h[hash_value];

		while (hash_cell != NULL && hash_cell->hashed_board != hash_value) {
			hash_cell = hash_cell->next;
		}

		if (hash_cell != NULL) {		// the node has already been explored before
			play_t *new_play = get_random_tile(hash_cell->plays);
			game->deck->n++;

			add_tile(play->tile);
			play->n_coup ++;
			int temp_score = simulation(new_play, h, game, !is_bot, is_last_node);
			play->gain_coup += temp_score;
			undo_tile(play->tile);

			// UNDO THE DECK
			game->deck->n--;
			return temp_score;

		} else {						// first time exploring this node, adding initialisation 
			linked_plays_t *lp = is_bot ? gen_tiles_from_game(game, is_bot) : gen_tiles_from_game(game, !is_bot);
			game->deck->n++;
			is_bot ? hash_map_add(h, game->bot, lp) : hash_map_add(h, game->player, lp);

			play_t *new_play = get_random_tile(lp);

			add_tile(play->tile);
			play->n_coup ++;
			int temp_score = simulation(new_play, h, game, !is_bot, is_last_node);
			play->gain_coup += temp_score;
			undo_tile(play->tile);

			// UNDO THE DECK
			game->deck->n--;
			return temp_score;
		}
	} else {							// random recursion until end game
		linked_plays_t *lp = is_bot ? gen_tiles_from_game(game, is_bot) : gen_tiles_from_game(game, !is_bot);
		game->deck->n++;

		play_t *new_play = get_random_tile(lp);

		add_tile(play->tile);
		int temp_score = simulation(new_play, h, game, !is_bot, is_last_node);
		undo_tile(play->tile);

		// UNDO THE DECK
		game->deck->n--;
		return temp_score;
	}
}

play_t *mcts(game_t *game) {
	srand(time(0)); // TO BE PUT IN MAIN
	int n = 1;	
	float c = sqrtf(2);

	hash_t **h = create_hash_map();
	play_t *p = initialisation(game, h);
	play_t *p2;


	time_t t0 = time(0);
	time_t t1 = time(0);
	while(difftime(t1, t0) < 10) {
		p2 = selection(p, c, n);

		simulation(p2, h, game, 0, 0);

		t1 = time(0);
		n++;
	}

	float gain = 0, gain_max = 0;
	play_t *max_play;

	while (p != NULL) {
		gain = G(p);
		if (gain > gain_max) {
			gain_max = gain;
			max_play = p;
		}

		p = p->next;
	}

	free_hash_map(h);

	return max_play;
}

