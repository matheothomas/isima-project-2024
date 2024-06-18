/* name : init.h
 * author : matheo thomas
 * date : 18-06-24
 */

#ifndef init_h 
#define init_h

/* Struct definitions */
typedef enum state {
	EMPTY,
	WHITE,
	BLACK,
} state_e;

typedef struct cell {
	state_e state;
	struct cell *neighbourg[6];
} cell_t;

typedef struct board {
	cell_t *cell;			// pointer to the first cell of the board
	int n_white;
	int n_black;
} board_t;



/* Functions definitions */

/// creates a cell of the board
cell_t *create_cell();
board_t create_clean_board();

#endif

