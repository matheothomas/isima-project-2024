/* name : init.c
 * author : matheo thomas
 * date : 18-06-24
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/init.h"

cell_t create_cell() {
	cell_t c;

	c.state = EMPTY;
	for(int i = 0; i < 6; i++) {
		c.neighbourg[i] = NULL;
	}

	return c;
}
