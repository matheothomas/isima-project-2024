/* name : init.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#ifndef init_h 
#define init_h

#define CELL_NUMBER 390
// #include "algos.h"


/* Struct definitions */
typedef enum cell_type {
	EMPTY,
	HOUSE_BLUE,
	MARKET_YELLOW,
	BARRAK_RED,
	TEMPLE_PURPLE,
	PARK_GREEN,
	QUARRY_GRAY,
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
	int yellow_mult;
	int red_mult;
	int purple_mult;
	int green_mult;
	int rocks;				// the money

} board_t;

typedef struct game {
	board_t player;
	board_t bot;
	tile_t *card_1;
	tile_t *card_2;
	struct play *deck;			// linked list of tiles
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
tile_t *create_tile();
void init_tile(tile_t * tile);

board_t *create_board();
void init_board(board_t * board);

tile_t ** creates_all_tiles();


#endif
