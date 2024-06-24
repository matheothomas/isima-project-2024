/* name : tests_eloi.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"
#include "init.h"
#include "tests_eloi.h"

void test_hash_index() {
	board_t * board = create_board();
	board -> score = 0;
	printf("hash index clean board %d\n", hash_index(board));
}
