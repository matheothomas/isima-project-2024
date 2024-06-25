/* name : init.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#ifndef init_h 
#define init_h

#define CELL_NUMBER 390
//#include "algos.h"


/* Struct definitions */
typedef enum cell_type {
	EMPTY,
	HOUSE_BLUE,
	BARRAK_RED,
	MARKET_YELLOW,
	TEMPLE_PURPLE,
	PARK_GREEN,
	QUARRY_GRAY,
	BLUE_PLACE,
	YELLOW_PLACE,
	RED_PLACE,
	PURPLE_PLACE,
	GREEN_PLACE,
} cell_type_e;

typedef struct type_linked {
	cell_type_e cell_type;
	struct tile *tile;
	struct type_linked *next;
} type_linked_t;



typedef struct cell {
	int id;
	type_linked_t *level;			// linked list of types
	struct cell *neighbour[6];
	// struct tile *parent; 
	int x, y;
	int altitude;
} cell_t;

typedef struct tile {
	int id;
	cell_t *cell_tab[3];
	cell_type_e cell_types[3];
	int orientation;
} tile_t;

typedef struct board {
	cell_t *cell;
	int blue_mult;
	int blue_nb;
	int yellow_mult;
	int yellow_nb;
	int red_mult;
	int red_nb;
	int purple_mult;
	int purple_nb;
	int green_mult;
	int green_nb;
	int rocks;				// the money
	int score;
	cell_t **cell_tab;

} board_t;

typedef struct game {
	board_t *player;
	board_t *bot;
	tile_t *card_1;
	tile_t *card_2;
	tile_t **deck;			// linked list of tiles
} game_t;

/// used for the different searches
typedef struct args {
	tile_t *tile;
	cell_t *cell;
	int score;
} args_t;


/* Functions definitions */

type_linked_t *create_type_linked();
void init_type_linked(type_linked_t * type_linked);

cell_t *create_cell();
void init_cell(cell_t * cell);

board_t *create_board();
void init_board(board_t * board);

tile_t *create_tile();
void init_tile(tile_t * tile);
void fill_tile(tile_t *tile, cell_type_e type_0, cell_type_e type_1, cell_type_e type_2);

tile_t ** creates_all_tiles();

args_t *create_and_init_args();
void updates_args(args_t *args, tile_t *tile, cell_t *cell, int score);

game_t * create_game();
void update_game(game_t *game, board_t *player, board_t *bot, tile_t *card_1, tile_t *card_2, tile_t **deck);

struct play * create_play();
void update_play(struct play *play, tile_t *tile, int n_coup, float gain_coup, struct play *next);


#endif
