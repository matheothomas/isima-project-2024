/* name : algos.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "algos.h"
#include "hash_map.h"
#include "init.h"
#include "utilities.h"


linked_plays_t *initialisation(game_t *game, hash_t **h) {
	linked_plays_t *lp = game->bot->rocks > 0 ? gen_tiles_from_game(game, true) : gen_tiles(game->bot->cell_tab, game->card_1);
	hash_map_add(h, game->bot, lp);

	return lp;
}

float G(play_t *play) {
	return play->gain_coup / (float)play->n_coup;
}

float interest(play_t *play, float c, int * n) {
	return G(play) + c * sqrtf(logf(*n) / play->n_coup);
}

play_t *ucb(play_t *play, float c, int * n, int * i, bool * working_nodes) {
	float temp_interest = 0, interest_max = 0;
	play_t *max_play = play;
	int j = 0;

	while (play != NULL) {
		temp_interest = interest(play, c, n);
		if (temp_interest > interest_max) {
			interest_max = temp_interest;
			max_play = play;
			*i = j;
		}
		j++;
		play = play->next;
	}
	
	working_nodes[*i] = true;

	return max_play;
}

play_t *selection(play_t *play, float c, int * n, int * i, bool * working_nodes) {

	play_t *temp = play;
	while(temp != NULL) {
		if (temp->n_coup == 0) {
			working_nodes[*i] = true;
			break;
		}
		temp = temp->next;
		(*i)++;
	}
	if (temp == NULL) {
		// UCB
		temp = ucb(play, c, n, i, working_nodes);
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
	
	// if(is_bot) {
		// update_scoring_table(game->bot);
		// calculate_score_from_table(game->bot);
	// } else {
		// update_scoring_table(game->player);
		// calculate_score_from_table(game->player);
	// }
	update_scoring_table_rec_false_start(is_bot ? game->bot : game->player);
	calculate_score_from_table(is_bot ? game->bot : game->player);
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
			play_t *new_play = get_random_tile(hash_cell->plays);
			game->deck->n++;

			is_bot ? add_tile_to_board_without_score(game->bot, play->tile) : add_tile_to_board_without_score(game->player, play->tile);
			play->n_coup ++;
			int temp_score = simulation(new_play, h, game, !is_bot, is_last_node);
			play->gain_coup += temp_score;
			is_bot ? remove_tile_from_board_without_null_without_score(game->bot, play->tile) : remove_tile_from_board_without_null_without_score(game->player, play->tile);

			// UNDO THE DECK
			game->deck->n--;
			return temp_score;

		} else {						// first time exploring this node, adding initialisation 
		if(is_bot) {
				lp = game->bot->rocks > 0 ? gen_tiles_from_game(game, true) : gen_tiles(game->bot->cell_tab, game->card_1);
			} else {
				lp = game->player->rocks > 0 ? gen_tiles_from_game(game, false) : gen_tiles(game->player->cell_tab, game->card_1);
			}
			game->deck->n++;
			is_bot ? hash_map_add(h, game->bot, lp) : hash_map_add(h, game->player, lp);

			play_t *new_play = get_random_tile(lp);

			is_bot ? add_tile_to_board_without_score(game->bot, play->tile) : add_tile_to_board_without_score(game->player, play->tile);

			play->n_coup ++;
			int temp_score = simulation(new_play, h, game, !is_bot, !is_last_node);
			play->gain_coup += temp_score;
			is_bot ? remove_tile_from_board_without_null_without_score(game->bot, play->tile) : remove_tile_from_board_without_null_without_score(game->player, play->tile);

			// UNDO THE DECK
			game->deck->n--;
			return temp_score;
		}
	} else {							// random recursion until end game
		if(is_bot) {
			lp = game->bot->rocks > 0 ? gen_tiles_from_game(game, true) : gen_tiles(game->bot->cell_tab, game->card_1);
		} else {
			lp = game->player->rocks > 0 ? gen_tiles_from_game(game, false) : gen_tiles(game->player->cell_tab, game->card_1);
		}
		game->deck->n++;

		play_t *new_play = get_random_tile(lp);

		is_bot ? add_tile_to_board_without_score(game->bot, play->tile) : add_tile_to_board_without_score(game->player, play->tile);
		int temp_score = simulation(new_play, h, game, !is_bot, is_last_node);
		is_bot ? remove_tile_from_board_without_null_without_score(game->bot, play->tile) : remove_tile_from_board_without_null_without_score(game->player, play->tile);

		// UNDO THE DECK
		game->deck->n--;
		return temp_score;
	}
}

play_t *copy_play(play_t *play) {
	play_t *new_play = malloc(sizeof(play_t));
	tile_t *tile = malloc(sizeof(tile_t));
	
	tile->id = play->tile->id;
	tile->cell_tab[0] = play->tile->cell_tab[0];
	tile->cell_tab[1] = play->tile->cell_tab[1];
	tile->cell_tab[2] = play->tile->cell_tab[2];
	tile->cell_types[0] = play->tile->cell_types[0];
	tile->cell_types[1] = play->tile->cell_types[1];
	tile->cell_types[2] = play->tile->cell_types[2];
	tile->id = play->tile->id;
	tile->orientation = play->tile->orientation;

	new_play->next = NULL;
	new_play->n_coup = play->n_coup;
	new_play->gain_coup = play->gain_coup;
	new_play->tile = tile;

	return new_play;
}

void * create_thread(void * args) {

	args_t * vals = args;
	play_t * p2 = NULL;

	while (*(vals -> pthread_state)) {
		int i = 0;
		if (pthread_mutex_lock(vals -> plays_mutex)) {
			fprintf(stderr, "Error locking tree_mutex in get_temp_address\n");
		}
		p2 = selection(vals -> p -> play, vals -> c, vals -> n, &i, vals -> working_nodes);
		printf("address %p %p\n", p2, vals -> p -> play);

		(*vals->n)++;
		if (pthread_mutex_unlock(vals -> plays_mutex)) {
			fprintf(stderr, "Error unlocking tree_mutex in get_temp_address\n");
		}

		simulation(p2, vals -> hash_map, vals -> game, 1, 0);
		vals -> working_nodes[i] = false;
	}

	return NULL;
}

play_t *mcts(game_t *game) {
	int n = 1;	
	float c = sqrtf(2)/10;

	hash_t **h = create_hash_map();
	linked_plays_t *p = initialisation(game, h);

	// THREADS BEWARE
	pthread_mutex_t play_mutex = PTHREAD_MUTEX_INITIALIZER;
	
	pthread_t threads[NUM_THREADS];
	args_t arguments[NUM_THREADS];
	bool pthread_states[NUM_THREADS] = {true};
	bool working_nodes[p -> size];

	for (int i = 0; i < NUM_THREADS; i++) {
		arguments[i].pthread_state = &pthread_states[i];
		arguments[i].game = game;
		arguments[i].p = p;
		arguments[i].hash_map = h;
		arguments[i].n = &n;
		arguments[i].c = c;
		arguments[i].plays_mutex = &play_mutex;
		arguments[i].working_nodes = working_nodes;
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		int flag = pthread_create(&threads[i], NULL, create_thread, (void *) &arguments[i]);
		if (flag) {
			fprintf(stderr, "Thread failed to initialize: %d\n", flag);
		}
	}

	sleep(2);

	// Signal the thread to stop
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_states[i] = false;
	}

	// Wait for all the threads to finish
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	float gain = 0;
	float gain_max = 0;
	play_t * cour = p -> play;
	play_t *max_play = cour;

	while (cour != NULL) {
		gain = G(cour);
		
		if (gain > gain_max) {
			gain_max = gain;
			max_play = cour;
		}

		cour = cour -> next;
	}

	max_play = copy_play(max_play);
	free_hash_map(h);

	printf("number of iterations : %d %p\n", n, max_play);

	return max_play;
}

