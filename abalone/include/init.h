/* name : init.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 18-06-24
 */

#ifndef init_h 
#define init_h
#define CELL_NUMBER 61

#define CELL_NUMBERS 61
#define MAX_DEPTH 4

/* Struct definitions */
typedef enum state {
	EMPTY,
	WHITE,
	BLACK,
} state_e;

typedef enum selection {
	UNSELECT,
	SELECT,
	MOUSE,
} selection_e;

typedef struct cell {
	int id;
	state_e state;
	selection_e selection;
	struct cell *neighbourg[6];
} cell_t;

typedef struct board {
	cell_t *cell;			// pointer to the first cell of the board, the one in the middle
	int n_white;
	int n_black;
} board_t;


/* Functions definitions */

/// creates a cell of the board
cell_t *create_cell(int id);

/// creates an empty board
board_t *create_clean_board();

/// puts balls on the board into a start configuration
board_t *start_config(board_t* b);

board_t* start_config_2(board_t* b);

/// creates a table with all cells of the board 
cell_t ** create_table(board_t b);

#endif

