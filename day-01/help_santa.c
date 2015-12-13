#include <stdio.h>

/*
 * Help santa with these AWFUL instructions.
 *  A matching pair of parenthesis cancel each other and may be removed.
 *  "Solo" parentheses tell him to go up "(" and down ")".
 */

int main(int argc, char * argv[]){
	int c;
	int current_floor = 0, instruction_count = 0;
	int danger_floor = -1, danger_floor_count = 0;

	while ((c = fgetc(stdin)) != EOF) {
		switch (c) {
			case('('):
				current_floor++;
				break;
			case(')'):
				current_floor--;
				break;
		}
		instruction_count++;
		if ((current_floor == danger_floor) && (danger_floor_count==0)) {
			fprintf(stderr, "Instruction %d made Santa descend into the depths.\n May he never return again in retail form....\n", instruction_count);
			danger_floor_count++;
		}
	}

	fprintf(stderr, "Santa might not want to go to floor: %d\n", current_floor);
	return 0;
}
