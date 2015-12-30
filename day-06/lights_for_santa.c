/*
 * toggle lights for Santa, create image of it.
 * Lights are in a 1000x1000 grid. Corner coordinates are 0,0, 0,999, 999,999, and 999,0.
 * Each coordinate pair represents opposite corners of a rectangle, inclusive; 0,0 to 2,2 refers to 9 lights in a 3x3 grid.
 * All lights start turned off.
 * Input file is of this style:
	turn on 887,9 through 959,629
	turn on 454,398 through 844,448
	turn off 539,243 through 559,965
	turn off 370,819 through 676,868
	turn off 145,40 through 370,997
	turn off 301,3 through 808,453
	turn on 351,678 through 951,908
	toggle 720,196 through 897,994
	toggle 831,394 through 904,860
	toggle 753,664 through 970,926
  *
  * Start by preprocessing the file to replace the words with single characters
  * cat input.txt | sed -e 's/turn on /+/g' -e 's/turn off /-/g' -e 's/toggle /|/g' -e 's/ through /,/g' > condensed_input.txt
  * Gives this:
	+887,9,959,629
	+454,398,844,448
	-539,243,559,965
	-370,819,676,868
	-145,40,370,997
	-301,3,808,453
	+351,678,951,908
	|720,196,897,994
	|831,394,904,860
	|753,664,970,926
  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_LIGHTS 1000

int main(int argc, char * argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Error: %s, provide the input filename.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// check that input file exists and is readable
	if (access(argv[1], F_OK | R_OK) == -1) {
		fprintf(stderr, "Error: inpu file \"%s\" doesn't exist or is not readable\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	FILE * input_fp;
	if ((input_fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Couldn't open input file: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	size_t ix = 0, iy = 0;
	unsigned char light_field[N_LIGHTS][N_LIGHTS] = {{0}};
	char light_operator = 'a';
	size_t X1 = 0, X2 = 0, Y1 = 0, Y2 = 0;

	while (fscanf(input_fp, "%c%zd,%zd,%zd,%zd\n", &light_operator,&X1,&Y1,&X2,&Y2) == 5) {
		// Make sure values match expected behavior for loops below
		if (X1 > X2) {
			fprintf(stderr, "Read %c %zu %zu %zu %zu\n", light_operator, X1, Y1, X2, Y2);
			fprintf(stderr, "X1 > X2\n");

		}
		if (Y1 > Y2) {
			fprintf(stderr, "Read %c %zu %zu %zu %zu\n", light_operator, X1, Y1, X2, Y2);
			fprintf(stderr, "Y1 > X2\n");
		}

		// fprintf(stderr, "Read %c %zu %zu %zu %zu\n", light_operator, X1, Y1, X2, Y2);

		// Do the lights
		for (ix = X1; ix <= X2; ix++) {
			for (iy = Y1; iy <= Y2; iy++) {
				// We have on (+), off (-), toggle (|)
				switch(light_operator) {
					case '+':
						light_field[ix][iy] = 1;
						break;
					case '-':
						light_field[ix][iy] = 0;
						break;
					case '|':
						light_field[ix][iy] ^= 1;
						break;
					default:
						fprintf(stderr, "PROBLEM. The input contained an instruction that is not part of the Santa-light-switching-protocol, this: %c\n", light_operator);
						exit(EXIT_FAILURE);
				}
			}
		}
	}

	fclose(input_fp);

	// Count how many lights are lit at the end
	size_t n_lit_lights = 0;
	for (ix = 0; ix < N_LIGHTS; ix++) {
		for (iy = 0; iy < N_LIGHTS; iy++) {
			if (light_field[ix][iy]) n_lit_lights++;
			// Scale the value for output to PGM format
			light_field[ix][iy] *= 255;
		}
	}

	FILE * image_fp = fopen("lights_for_santa.pgm", "wb");
	fprintf(image_fp, "P5\n%d %d\n255\n", N_LIGHTS, N_LIGHTS);
	fwrite(light_field, sizeof(light_field), 1, image_fp);
	fclose(image_fp);

	printf("There are %zu lit lights\n", n_lit_lights);
	return(0);
}
