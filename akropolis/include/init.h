/* name : init.h
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#ifndef init_h 
#define init_h

/* Struct definitions */
typedef enum cell_type {
	HOUSE_BLUE,
	MARKET_YELLOW,
	BARRAK_RED,
	TEMPLE_PURPLE,
	PARK_GREEN,
	QUARRY_GRAY,
} cell_type_e;


typedef struct cell {
	int id;
	cell_type_e cell_type;
	struct cell *neighbour[6];
	struct tile *parent;
	int x, y;
	int altitude;

} cell_t;

typedef struct tile {
	int id;
	cell_t *cell_tab[3];
	int orientation;

} tile_t;

typedef struct board {


} board_t;


/* Functions definitions */


#endif
