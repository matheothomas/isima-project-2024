/* name : main.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tests_domitille.h"
#include "tests_eloi.h"
#include "tests_matheo.h"


int main(int argc, char *argv[]){
	if(argc == 2) {
		if(strcmp(argv[1], "domitille") == 0) {
			printf("domitille\n");
			test_domi();

		} else if(strcmp(argv[1], "eloi") == 0) {
			printf("eloi\n");
			test_add_tiles();

		} else if(strcmp(argv[1], "matheo") == 0) {
			printf("matheo\n");

			test_matheo();

			
		}
	} else {





	}

	return 0;
}

