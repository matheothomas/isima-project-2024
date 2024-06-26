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
#include "hash_map.h"
#include "init.h"
#include "utilities.h"


play_t *initialisation(game_t *game, hash_t **h) {
	linked_plays_t *lp = game->bot->rocks > 0 ? gen_tiles_from_game(game, true) : gen_tiles(game->bot->cell_tab, game->card_1);
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
	play_t *max_play = NULL;


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

	/*
	play_t *cour = lp->play;
	for (int i = 0 ; i < 3; i++) {
		if (p->tile->cell_tab[i] == NULL) {
			for(int j = 0; j < lp->size; j++) {
				printf("lp[%d] : %d %d %d\n", j, cour->tile->cell_types[0], cour->tile->cell_types[1], cour->tile->cell_types[2]);
				printf("lp[%d] : %p %p %p\n", j, cour->tile->cell_tab[0], cour->tile->cell_tab[1], cour->tile->cell_tab[2]);
				cour = cour->next;
			}
			break;
		}
	}
	*/
	return p;
}

bool is_game_finished(game_t *game) {
	return game->deck->n == DECK_NUMBER ? true : false;
}

int calculate_score(game_t *game, bool is_bot) {
	return is_bot ? game->bot->score - game->player->score : game->player->score - game->bot->score;
}

int simulation(play_t *play, hash_t **h, game_t *game, bool is_bot, bool is_last_node) {
	linked_plays_t *lp = NULL;
	if(is_game_finished(game)) {
		return calculate_score(game, is_bot);
	}

	if(!is_last_node) {
		board_t *b = is_bot ? game->bot : game->player;

		uint32_t hash_value = hash_board(b);

		int hash_index = hash_value % HASHMAP_SIZE;

		hash_t *hash_cell = h[hash_index];

		while (hash_cell != NULL && hash_cell->hashed_board != hash_value) {
			hash_cell = hash_cell->next;
		}

		if (hash_cell != NULL) {		// the node has already been explored before
			printf("node already explored\n");
			play_t *new_play = get_random_tile(hash_cell->plays);
			game->deck->n++;

			// add_tile(play->tile);
			is_bot ? add_tile_to_board(game->bot, play->tile) : add_tile_to_board(game->player, play->tile);
			play->n_coup ++;
			int temp_score = simulation(new_play, h, game, !is_bot, is_last_node);
			play->gain_coup += temp_score;
			is_bot ? remove_tile_from_board_without_null(game->bot, play->tile) : remove_tile_from_board_without_null(game->player, play->tile);
			// undo_tile(play->tile);

			// UNDO THE DECK
			game->deck->n--;
			return temp_score;

		} else {						// first time exploring this node, adding initialisation 
			// linked_plays_t *lp = is_bot ? gen_tiles_from_game(game, is_bot) : gen_tiles_from_game(game, !is_bot);
			printf("first time exploring node\n");
		if(is_bot) {
				lp = game->bot->rocks > 0 ? gen_tiles_from_game(game, true) : gen_tiles(game->bot->cell_tab, game->card_1);
			} else {
				lp = game->player->rocks > 0 ? gen_tiles_from_game(game, false) : gen_tiles(game->player->cell_tab, game->card_1);
			}
			game->deck->n++;
			is_bot ? hash_map_add(h, game->bot, lp) : hash_map_add(h, game->player, lp);

			play_t *new_play = get_random_tile(lp);

			// tile_t *temp_tile = play->tile;
			// bool is_broken = false;
			// for(int i = 0; i < 3; i++) {
			// if(temp_tile->cell_tab[i] == NULL) {
			// is_broken = 1;
			// }
			// }

			// if(!is_broken) {
			// add_tile(play->tile);
			is_bot ? add_tile_to_board(game->bot, play->tile) : add_tile_to_board(game->player, play->tile);

			// }
			play->n_coup ++;
			int temp_score = simulation(new_play, h, game, !is_bot, !is_last_node);
			play->gain_coup += temp_score;
			// if(!is_broken) {
			// undo_tile(play->tile);
			// }
			is_bot ? remove_tile_from_board_without_null(game->bot, play->tile) : remove_tile_from_board_without_null(game->player, play->tile);

			// UNDO THE DECK
			game->deck->n--;
			return temp_score;
		}
	} else {							// random recursion until end game
		// linked_plays_t *lp = is_bot ? gen_tiles_from_game(game, is_bot) : gen_tiles_from_game(game, !is_bot);
		printf("random recursion until end game\n");
		if(is_bot) {
			lp = game->bot->rocks > 0 ? gen_tiles_from_game(game, true) : gen_tiles(game->bot->cell_tab, game->card_1);
		} else {
			lp = game->player->rocks > 0 ? gen_tiles_from_game(game, false) : gen_tiles(game->player->cell_tab, game->card_1);
		}
		game->deck->n++;

		play_t *new_play = get_random_tile(lp);

		// add_tile(play->tile);
		is_bot ? add_tile_to_board(game->bot, play->tile) : add_tile_to_board(game->player, play->tile);
		int temp_score = simulation(new_play, h, game, !is_bot, is_last_node);
		// undo_tile(play->tile);
		is_bot ? remove_tile_from_board_without_null(game->bot, play->tile) : remove_tile_from_board_without_null(game->player, play->tile);

		// UNDO THE DECK
		game->deck->n--;
		return temp_score;
	}
}

play_t *mcts(game_t *game) {
	int n = 1;	
	float c = sqrtf(2)/10;

	hash_t **h = create_hash_map();
	play_t *p = initialisation(game, h);
	play_t *p2 = NULL;


	time_t t0 = time(0);
	time_t t1 = time(0);
	while(difftime(t1, t0) < 2) {
		p2 = selection(p, c, n);

		simulation(p2, h, game, 0, 0);

		t1 = time(0);
		printf("n : %d\n", n);
		n++;
	}

	float gain = 0;
	float gain_max = 0;
	play_t *max_play = NULL;

	while (p != NULL) {
		gain = G(p);
		// if(1 > 0) {
		if (gain > gain_max) {
			gain_max = gain;
			max_play = p;
		}

		p = p->next;
	}

	// free_hash_map(h);
	printf("number of iterations : %d\n", n);

	return max_play;
}

