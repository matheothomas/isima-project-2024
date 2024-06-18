#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/init.h"
#include "../include/algos.h"
#include "../include/utilities.h"


bool validity_play(board_t * board, play_t * play) {
	
}

int traversal_rec(board_t * board, play_t * play, cell_t * cell, int * visited, int cell_direction) {

	if (!visited[cell -> id]) {
		visited[cell -> id] = 1;

		switch (cell -> state) {
			case EMPTY:
				if (play == NULL) {
					for (int i = 0; i < 6; i++) {
						traversal_rec(board, NULL, (cell -> neighbourg)[i], visited, i);
					}
				}
				else {
					validity_play(board, play);
				}
				break;

			case WHITE:
				if (play -> cell_tab_length == 0) {
					play -> cell_tab_length ++;
					play -> cell_tab[0] = cell;
					for (int i = 0; i < 6; i++) {
						play_t new_play;
						new_play.cell_tab_length = 0;
						new_play.movement_direction = -1;
						new_play.cell_tab[0] = cell;
						traversal_rec(board, &new_play, cell -> neighbourg[i], visited, i);
					}
				}
				else if (play -> cell_tab_length < 3) {
					play -> cell_tab[play -> cell_tab_length] = cell;
					play -> cell_tab_length ++;
					if (play -> cell_tab_length == 1) {
						play -> movement_direction = cell_direction
					}
				}
				
				break;

			case BLACK:
				
				break;
		}
	}

}

int traversal(board_t * board) {

	int visited[CELL_NUMBER] = {0};



	return EXIT_SUCCESS;
}
