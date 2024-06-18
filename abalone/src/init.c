/* name : init.c
 * author : matheo thomas
 * date : 18-06-24
 */

#include "../include/init.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

cell_t *create_cell() {
  cell_t *c = malloc(sizeof(cell_t));
  c->state = EMPTY;

  for (int i = 0; i < 6; i++) {
    c->neighbourg[i] = NULL;
  }

  return c;
}

board_t create_clean_board() {

  board_t b;
  cell_t *first_cell = create_cell();
  b.cell = first_cell;
  b.n_black = 0;
  b.n_white = 0;

  cell_t *cur_cell = b.cell;
  int j = 0;
  int c = 0;

  // initialisation of the first cell, the one in the center.

  cur_cell->state = EMPTY;
  for (int i = 0; i < 6; i++) {
    cur_cell->neighbourg[i] = create_cell();
    cur_cell->neighbourg[i]->neighbourg[(i + 3) % 6] = cur_cell;
  }

  // creation of other cells

  for (int i = 0; i < 6; i++) {
    c = 0;
    j = 0;
    cur_cell = b.cell->neighbourg[i];

    while (c < 3) {

      for (int n = 0; n < 6; n++) {
        if (n != 3 && ((c != 0) || (c == 0 && n != 4))) {
          if (n == 2) {
            cur_cell->neighbourg[(i + n) % 6] =
                cur_cell->neighbourg[(i + 3) % 6]->neighbourg[(i + 1) % 6];
            cur_cell->neighbourg[(i + n) % 6]->neighbourg[(i + n + 3) % 6] =
                cur_cell;
          } else if (n == 4 && i == 0) {
            cur_cell->neighbourg[(i + n) % 6] =
                cur_cell->neighbourg[(i + 3) % 6]->neighbourg[(i + 5) % 6];
            cur_cell->neighbourg[(i + n) % 6]->neighbourg[(i + n + 3) % 6] =
                cur_cell;
          } else {
            if (n == 5 && i != 0) {
              cur_cell->neighbourg[(i + n) % 6] =
                  cur_cell->neighbourg[(i + n + 5) % 6]->neighbourg[i];
              cur_cell->neighbourg[(i + n) % 6]->neighbourg[(i + n + 3) % 6] =
                  cur_cell;
            } else if (n == 4 && i != 0) {
              cur_cell->neighbourg[(i + n) % 6] =
                  cur_cell->neighbourg[(i + n + 5) % 6]
                      ->neighbourg[(i + 5) % 6];
              cur_cell->neighbourg[(i + n) % 6]->neighbourg[(i + n + 3) % 6] =
                  cur_cell;
            } else {
              if (i == 5 && n == 1 && c == 0) {
                cur_cell->neighbourg[(i + n) % 6] =
                    b.cell->neighbourg[0]->neighbourg[5];
                cur_cell->neighbourg[(i + n) % 6]->neighbourg[(i + n + 3) % 6] =
                    cur_cell;
              } else {
                cur_cell->neighbourg[(i + n) % 6] = create_cell();
                cur_cell->neighbourg[(i + n) % 6]->neighbourg[(i + n + 3) % 6] =
                    cur_cell;
              }
            }
          }
        }
      }

      cur_cell = cur_cell->neighbourg[i];
      c += 1;
    }

    c = 0;
    j = 2;

    cur_cell->neighbourg[i] = NULL;
    cur_cell->neighbourg[(i + 1) % 6] = NULL;
    cur_cell->neighbourg[(i + 5) % 6] = NULL;

    cur_cell->neighbourg[(i + 2) % 6] =
        cur_cell->neighbourg[(i + 3) % 6]->neighbourg[(i + 1) % 6];
    cur_cell->neighbourg[(i + 2) % 6]->neighbourg[(i + 2 + 3) % 6] = cur_cell;

    cur_cell->neighbourg[(i + 4) % 6] =
        cur_cell->neighbourg[(i + 3) % 6]->neighbourg[(i + 5) % 6];
    cur_cell->neighbourg[(i + 4) % 6]->neighbourg[(i + 4 + 3) % 6] = cur_cell;

    while (c < 2) {
      cur_cell = cur_cell->neighbourg[(i + j) % 6];
      cur_cell->neighbourg[i] = NULL;
      cur_cell->neighbourg[(i + 1) % 6] = NULL;
      if (c == 1 && i == 5) {
        cur_cell->neighbourg[(i + 2) % 6] =
            b.cell->neighbourg[0]->neighbourg[0]->neighbourg[0]->neighbourg[5];
        cur_cell->neighbourg[(i + 2) % 6]->neighbourg[(i + 2 + 3) % 6] =
            cur_cell;

        cur_cell->neighbourg[(i + 3) % 6] =
            b.cell->neighbourg[0]->neighbourg[0]->neighbourg[5];
        cur_cell->neighbourg[(i + 3) % 6]->neighbourg[(i + 3 + 3) % 6] =
            cur_cell;

        cur_cell->neighbourg[(i + 4) % 6] =
            cur_cell->neighbourg[(i + 5) % 6]->neighbourg[(i + 3) % 6];
        cur_cell->neighbourg[(i + 4) % 6]->neighbourg[(i + 4 + 3) % 6] =
            cur_cell;

      } else {
        cur_cell->neighbourg[(i + 2) % 6] = create_cell();
        cur_cell->neighbourg[(i + 2) % 6]->neighbourg[(i + 2 + 3) % 6] =
            cur_cell;

        if (c == 0) {
          cur_cell->neighbourg[(i + 3) % 6] =
              cur_cell->neighbourg[(i + 4) % 6]->neighbourg[(i + 2) % 6];
          cur_cell->neighbourg[(i + 3) % 6]->neighbourg[(i + 3 + 3) % 6] =
              cur_cell;
        } else {
          cur_cell->neighbourg[(i + 3) % 6] = create_cell();
          cur_cell->neighbourg[(i + 3) % 6]->neighbourg[(i + 3 + 3) % 6] =
              cur_cell;
          cur_cell->neighbourg[(i + 4) % 6] =
              cur_cell->neighbourg[(i + 5) % 6]->neighbourg[(i + 3) % 6];
          cur_cell->neighbourg[(i + 4) % 6]->neighbourg[(i + 4 + 3) % 6] =
              cur_cell;
        }
      }

      c += 1;
    }

    c = 0;
    j = 3;

    cur_cell = cur_cell->neighbourg[(i + j) % 6];

    cur_cell->neighbourg[(i + 1) % 6] =
        cur_cell->neighbourg[i]->neighbourg[(i + 2) % 6];
    cur_cell->neighbourg[(i + 1) % 6]->neighbourg[(i + 1 + 3) % 6] = cur_cell;

    cur_cell->neighbourg[(i + 5) % 6] =
        cur_cell->neighbourg[i]->neighbourg[(i + 4) % 6];
    cur_cell->neighbourg[(i + 5) % 6]->neighbourg[(i + 5 + 3) % 6] = cur_cell;

    cur_cell->neighbourg[(i + 4) % 6] = cur_cell->neighbourg[(i + 5) % 6]
                                            ->neighbourg[(i + 4) % 6]
                                            ->neighbourg[(i + 2) % 6];
    cur_cell->neighbourg[(i + 4) % 6]->neighbourg[(i + 4 + 3) % 6] = cur_cell;

    j = 4;

    cur_cell = cur_cell->neighbourg[(i + j) % 6];

    cur_cell->neighbourg[i] =
        cur_cell->neighbourg[(i + 1) % 6]->neighbourg[(i + 5) % 6];
    cur_cell->neighbourg[i]->neighbourg[(i + 3) % 6] = cur_cell;
  }
  return b;
}
