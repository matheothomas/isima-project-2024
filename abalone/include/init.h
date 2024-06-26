/* name : init.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 18-06-24
 */

#ifndef init_h 
#define init_h
#define CELL_NUMBER 61

#define MAX_DEPTH 4

/* Struct definitions */ 

/// state of a cell on the board
typedef enum state {
	EMPTY,
	WHITE,
	BLACK,
} state_e;

/// state of selection of a cell on the board
typedef enum selection {
	UNSELECT,
	SELECT,
	MOUSE,
} selection_e;

/// cell of the board
typedef struct cell {
	int id;
	state_e state;
	selection_e selection;
	struct cell *neighbor[6];
} cell_t;

/// board containing the number of cells for each player, and a pointer to the middle cell
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

/// puts balls on the board into another start configuration
board_t* start_config_2(board_t* b);

/// creates an array with all the cells of the board 
cell_t ** create_table(board_t b);

void free_cell(cell_t *cell);

void free_board_and_cell_tab(board_t *board, cell_t **cell_tab);

#endif

