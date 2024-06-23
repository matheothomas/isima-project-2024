/* name : main.c
 * authors : eloi petit, matheo thomas, domitille vale
 * date : 23-06-24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){
	if(argc == 2) {
		if(strcmp(argv[1], "domitille") == 0) {
			printf("domitille\n");
		} else if(strcmp(argv[1], "eloi") == 0) {
			printf("eloi\n");
		} else if(strcmp(argv[1], "matheo") == 0) {
			printf("matheo\n");
		}
	} else {

	}

	return 0;
}

