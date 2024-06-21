/* name : init.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 18-06-24
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "init.h"

cell_t *create_cell(int id) {
	cell_t *c = malloc(sizeof(cell_t));
	c->state = EMPTY;
	c->id=id;
	c->selection=UNSELECT;

	for (int i = 0; i < 6; i++) {
		c->neighbor[i] = NULL;
	}

	return c;
}

board_t* create_clean_board() {

	// initialisation of the board
	board_t*ptb=(board_t*)malloc(sizeof(board_t));
	cell_t *first_cell = create_cell(0);
	ptb->cell = first_cell;
	ptb->n_black = 0;
	ptb->n_white = 0;

	cell_t *cur_cell = ptb->cell;
	int j = 0;
	int c = 0;
	int i=0;

	// initialisation of the first cell, the one in the center.

	int id = 0;
	cur_cell->state = EMPTY;
	for (i = 0; i < 6; i++) {
		id++;
		cur_cell->neighbor[i] = create_cell(id);
		cur_cell->neighbor[i]->neighbor[(i + 3) % 6] = cur_cell;
	}

	// creation of other cells

	for (i = 0; i < 6; i++) {
		c = 0;
		j = 0;
		cur_cell = ptb->cell->neighbor[i];

		while (c < 3) {

			for (int n = 0; n < 6; n++) {
				if (n != 3 && ((c != 0) || (c == 0 && n != 4))) {
					if (n == 2) {
						cur_cell->neighbor[(i + n) % 6] =
							cur_cell->neighbor[(i + 3) % 6]->neighbor[(i + 1) % 6];
						cur_cell->neighbor[(i + n) % 6]->neighbor[(i + n + 3) % 6] =
						cur_cell;
					} else if (n == 4 && i == 0) {
						cur_cell->neighbor[(i + n) % 6] =
							cur_cell->neighbor[(i + 3) % 6]->neighbor[(i + 5) % 6];
						cur_cell->neighbor[(i + n) % 6]->neighbor[(i + n + 3) % 6] =
						cur_cell;
					} else {
						if (n == 5 && i != 0) {
							cur_cell->neighbor[(i + n) % 6] =
								cur_cell->neighbor[(i + n + 5) % 6]->neighbor[i];
							cur_cell->neighbor[(i + n) % 6]->neighbor[(i + n + 3) % 6] =
							cur_cell;
						} else if (n == 4 && i != 0) {
							cur_cell->neighbor[(i + n) % 6] =
								cur_cell->neighbor[(i + n + 5) % 6]
								->neighbor[(i + 5) % 6];
							cur_cell->neighbor[(i + n) % 6]->neighbor[(i + n + 3) % 6] =
							cur_cell;
						} else {
							if (i == 5 && n == 1 && c == 0) {
								cur_cell->neighbor[(i + n) % 6] =
									ptb->cell->neighbor[0]->neighbor[5];
								cur_cell->neighbor[(i + n) % 6]->neighbor[(i + n + 3) % 6] =
								cur_cell;
							} else {
								id++;
								cur_cell->neighbor[(i + n) % 6] = create_cell(id);
								cur_cell->neighbor[(i + n) % 6]->neighbor[(i + n + 3) % 6] =
								cur_cell;
							}
						}
					}
				}
			}

			cur_cell = cur_cell->neighbor[i];
			c += 1;
		}

		c = 0;
		j = 2;

		cur_cell->neighbor[i] = NULL;
		cur_cell->neighbor[(i + 1) % 6] = NULL;
		cur_cell->neighbor[(i + 5) % 6] = NULL;

		cur_cell->neighbor[(i + 2) % 6] =
			cur_cell->neighbor[(i + 3) % 6]->neighbor[(i + 1) % 6];
		cur_cell->neighbor[(i + 2) % 6]->neighbor[(i + 2 + 3) % 6] = cur_cell;

		cur_cell->neighbor[(i + 4) % 6] =
			cur_cell->neighbor[(i + 3) % 6]->neighbor[(i + 5) % 6];
		cur_cell->neighbor[(i + 4) % 6]->neighbor[(i + 4 + 3) % 6] = cur_cell;

		while (c < 2) {
			cur_cell = cur_cell->neighbor[(i + j) % 6];
			cur_cell->neighbor[i] = NULL;
			cur_cell->neighbor[(i + 1) % 6] = NULL;
			if (c == 1 && i == 5) {
				cur_cell->neighbor[(i + 2) % 6] =
					ptb->cell->neighbor[0]->neighbor[0]->neighbor[0]->neighbor[5];
				cur_cell->neighbor[(i + 2) % 6]->neighbor[(i + 2 + 3) % 6] =
				cur_cell;

				cur_cell->neighbor[(i + 3) % 6] =
					ptb->cell->neighbor[0]->neighbor[0]->neighbor[5];
				cur_cell->neighbor[(i + 3) % 6]->neighbor[(i + 3 + 3) % 6] =
				cur_cell;

				cur_cell->neighbor[(i + 4) % 6] =
					cur_cell->neighbor[(i + 5) % 6]->neighbor[(i + 3) % 6];
				cur_cell->neighbor[(i + 4) % 6]->neighbor[(i + 4 + 3) % 6] =
				cur_cell;

			} else {
				id ++;
				cur_cell->neighbor[(i + 2) % 6] = create_cell(id);
				cur_cell->neighbor[(i + 2) % 6]->neighbor[(i + 2 + 3) % 6] =
				cur_cell;

				if (c == 0) {
					cur_cell->neighbor[(i + 3) % 6] =
						cur_cell->neighbor[(i + 4) % 6]->neighbor[(i + 2) % 6];
					cur_cell->neighbor[(i + 3) % 6]->neighbor[(i + 3 + 3) % 6] =
					cur_cell;
				} else {
					id++;
					cur_cell->neighbor[(i + 3) % 6] = create_cell(id);
					cur_cell->neighbor[(i + 3) % 6]->neighbor[(i + 3 + 3) % 6] =
					cur_cell;
					cur_cell->neighbor[(i + 4) % 6] =
						cur_cell->neighbor[(i + 5) % 6]->neighbor[(i + 3) % 6];
					cur_cell->neighbor[(i + 4) % 6]->neighbor[(i + 4 + 3) % 6] =
					cur_cell;
				}
			}

			c += 1;
		}

		c = 0;
		j = 3;

		cur_cell = cur_cell->neighbor[(i + j) % 6];

		cur_cell->neighbor[(i + 1) % 6] =
			cur_cell->neighbor[i]->neighbor[(i + 2) % 6];
		cur_cell->neighbor[(i + 1) % 6]->neighbor[(i + 1 + 3) % 6] = cur_cell;

		cur_cell->neighbor[(i + 5) % 6] =
			cur_cell->neighbor[i]->neighbor[(i + 4) % 6];
		cur_cell->neighbor[(i + 5) % 6]->neighbor[(i + 5 + 3) % 6] = cur_cell;

		cur_cell->neighbor[(i + 4) % 6] = cur_cell->neighbor[(i + 5) % 6]
			->neighbor[(i + 4) % 6]
			->neighbor[(i + 2) % 6];
		cur_cell->neighbor[(i + 4) % 6]->neighbor[(i + 4 + 3) % 6] = cur_cell;

		j = 4;

		cur_cell = cur_cell->neighbor[(i + j) % 6];

		cur_cell->neighbor[i] =
			cur_cell->neighbor[(i + 1) % 6]->neighbor[(i + 5) % 6];
		cur_cell->neighbor[i]->neighbor[(i + 3) % 6] = cur_cell;
	}

	return ptb;
}

board_t* start_config(board_t* b) {
	cell_t *cur_cell;
	state_e colour = WHITE;
	b->n_black = 14;
	b->n_white = 14;

	for (int n = 0; n < 4; n += 3) {

		cur_cell = b->cell->neighbor[n]->neighbor[n]->neighbor[n]->neighbor[n];
		for (int i = 0; i < 4; i++) {
			cur_cell->state = colour;
			cur_cell = cur_cell->neighbor[(n + 2) % 6];
		}
		cur_cell->state = colour;

		cur_cell = cur_cell->neighbor[(n + 3) % 6];
		for (int i = 0; i < 5; i++) {
			cur_cell->state = colour;
			cur_cell = cur_cell->neighbor[(n + 5) % 6];
		}
		cur_cell->state = colour;

		cur_cell = cur_cell->neighbor[(n + 3) % 6]->neighbor[(n + 2) % 6];
		for (int i = 0; i < 3; i++) {
			cur_cell->state = colour;
			cur_cell = cur_cell->neighbor[(n + 2) % 6];
		}

		colour = BLACK;
	}
	return b;
}

board_t* start_config_2(board_t* b) {
	cell_t *cur_cell;
	state_e colour = WHITE;
	int i=0;
	b->n_black = 14;
	b->n_white = 14;

	for (int n = 0; n < 4; n += 3) {

		cur_cell = b->cell->neighbor[n]->neighbor[n]->neighbor[n]->neighbor[n];
		for (i = 0; i < 4; i++) {
			cur_cell->state = colour;
			cur_cell = cur_cell->neighbor[(n + 2) % 6];
		}
		cur_cell->state = colour;

		cur_cell = cur_cell->neighbor[(n + 4) % 6];
		for (i = 0; i < 3; i++) {
			cur_cell->state = colour;
			cur_cell = cur_cell->neighbor[(n + 5) % 6];
		}
		cur_cell->state = colour;

		cur_cell = cur_cell->neighbor[(n + 3) % 6];
		for (i = 0; i < 2; i++) {
			cur_cell->state = colour;
			cur_cell = cur_cell->neighbor[(n + 2) % 6];
		}
		cur_cell->state = colour;

		cur_cell = cur_cell->neighbor[(n + 4) % 6];
		for (i = 0; i < 2; i++) {
			cur_cell->state = colour;
			cur_cell = cur_cell->neighbor[(n + 5) % 6];
		}
		colour = BLACK;
	}
	return b;
}

cell_t ** create_table(board_t b) {
	cell_t **table=(cell_t**)malloc(61 * sizeof(cell_t*));
	cell_t* cur_cell;
	int i=0;

	cur_cell = b.cell->neighbor[0]->neighbor[0]->neighbor[0]->neighbor[0];
	for (i = 0; i < 5; i++) {
		table[i] = cur_cell;
		cur_cell = cur_cell->neighbor[2];
	}

	cur_cell = b.cell->neighbor[0]->neighbor[0]->neighbor[0]->neighbor[5];
	for (i = 5; i < 11; i++) {
		table[i] = cur_cell;
		cur_cell = cur_cell->neighbor[2];
	}

	cur_cell = b.cell->neighbor[0]->neighbor[0]->neighbor[5]->neighbor[5];
	for (i = 11; i < 18; i++) {
		table[i] = cur_cell;
		cur_cell = cur_cell->neighbor[2];
	}

	cur_cell = b.cell->neighbor[0]->neighbor[5]->neighbor[5]->neighbor[5];
	for (i = 18; i < 26; i++) {
		table[i] = cur_cell;
		cur_cell = cur_cell->neighbor[2];
	}

	cur_cell = b.cell->neighbor[5]->neighbor[5]->neighbor[5]->neighbor[5];
	for (i = 26; i < 35; i++) {
		table[i] = cur_cell;
		cur_cell = cur_cell->neighbor[2];
	}

	cur_cell = b.cell->neighbor[4]->neighbor[5]->neighbor[5]->neighbor[5];
	for (i = 35; i < 43; i++) {
		table[i] = cur_cell;
		cur_cell = cur_cell->neighbor[2];
	}

	cur_cell = b.cell->neighbor[4]->neighbor[4]->neighbor[5]->neighbor[5];
	for (i = 43; i < 50; i++) {
		table[i] = cur_cell;
		cur_cell = cur_cell->neighbor[2];
	}

	cur_cell = b.cell->neighbor[4]->neighbor[4]->neighbor[4]->neighbor[5];
	for (i = 50; i < 56; i++) {
		table[i] = cur_cell;
		cur_cell = cur_cell->neighbor[2];
	}
	
	cur_cell = b.cell->neighbor[4]->neighbor[4]->neighbor[4]->neighbor[4];
	for (i = 56; i < 61; i++) {
		table[i] = cur_cell;
		cur_cell = cur_cell->neighbor[2];
	}
	return table;
}
