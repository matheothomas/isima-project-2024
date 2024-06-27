/* name : algos.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#ifndef algos_h 
#define algos_h

#include <pthread.h>
#include "init.h"
#include "utilities.h"
#include "hash_map.h"

#define NUM_THREADS 1

/* Struct definitions */

/// Used to pass arguments to threads
typedef struct args {
	bool * pthread_state;
	game_t * game;
	linked_plays_t * p;
	hash_t ** hash_map;
	int * n;
	int c;
	pthread_mutex_t * plays_mutex;
	bool * working_nodes;
} args_t;


/* Functions definitions */

/// Itinialises the root of the tree with the two tiles available.
linked_plays_t *initialisation(game_t *game, hash_t **h);

/// Selects the play to play.
play_t *selection(play_t *play, float c, int * n, int * i, bool * working_nodes);

/// Returns a random tile from a play list.
play_t *get_random_tile(linked_plays_t *lp);

/// Simulates the plays and updates their scores.
int simulation(play_t *play, hash_t **h, game_t *game, bool is_bot, bool is_last_node);

///
play_t *ucb(play_t *play, float c, int * n, int * i, bool * working_nodes);

/// Applies the MCTS algorithm for a fixed time.
play_t *mcts(game_t *game);



#endif

