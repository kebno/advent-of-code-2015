#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Use the input file as instructions for traversing a 2D grid.
 * Create an image to show the path taken for these instructions.
 */

int main(int argc, char * argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Error: %s, provide an input filename.\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	// check that the input file exists and is readable
	if (access(argv[1], F_OK | R_OK) == -1) {
		fprintf(stderr, "Error: input file \"%s\" doesn't exist or is not readable\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	// Form command to count number of characters in the input file
	char * command = NULL;
	asprintf(&command, "/usr/bin/wc -m %s", argv[1]);
	printf("The command to be run: %s\n", command);

	// Run command
	FILE * fp;
	if ((fp = popen(command, "r")) == NULL) {
		fprintf(stderr, "Failed to run command: %s\n", command);
		exit(EXIT_FAILURE);
	}

	// Read the command result (something like:  "1024 input.txt")
	char output[24];
	if (fgets(output, sizeof(output)-1, fp) == NULL) {
		fprintf(stderr, "Error: could not read count of characters from this command: %s\n", command);
		pclose(fp);
		exit(EXIT_FAILURE);
	}
	pclose(fp);

	// Get the number out of the output string
	size_t num_characters = 0;
	sscanf(output, "%zu", &num_characters);
	fprintf(stderr, "Number of characters in %s: %zu\n", argv[1], num_characters);

	// Setup and create the grid
	const size_t X = num_characters/32, Y = num_characters/32, offset = X/2;
	size_t xpos = 0, ypos = 0;
	size_t ix, iy;
	int * house_grid = calloc(X * Y, sizeof(int));

	// // Ask if it is okay to produce the potentially large image
	// printf("This program will create an image of about %.0f MB\n", 3 * X * Y * 1e-6);
	// printf("Are you okay with that? (y/n)\n");
	// if (getc(stdin) == 'n') {
	// 	printf("Okay. Exiting...\n");
	// 	exit(0);
	// }

	// Start at center house
	xpos = offset;
	ypos = offset;
	house_grid[xpos + ypos * Y] += 1;

	// Visit houses by reading and following the instructions FTW!
	if ((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Couldn't open input file: %s\n",argv[1]);
		exit(EXIT_FAILURE);
	}
	int next_instruction = 0;
	size_t instruction_count = 0;
	while ((next_instruction = fgetc(fp)) != EOF) {
		switch (next_instruction) {
			case '^':
				ypos++;
				break;
			case 'v':
				ypos--;
				break;
			case '>':
				xpos++;
				break;
			case '<':
				xpos--;
				break;
			case '\n':
			case ' ':
				break;
			default:
				fprintf(stderr, "PROBLEM. Santa doesn't understand this instruction: \"%c\"\n", next_instruction);
				fprintf(stderr, "Fix it, fix it, fix it, and Try Again!!!!\n");
				exit(EXIT_FAILURE);
		}
		if ((xpos + ypos * Y) > ((X-1) * (Y-1))) {
			fprintf(stderr, "Santa went out of the expected grid.\n");
			fprintf(stderr, "Please change the sizes \"X\" and \"Y\" in the source code.\n");
			exit(EXIT_FAILURE);
		}
		house_grid[xpos + ypos * Y] += 1;
		instruction_count++;

	}
	fclose(fp);

	// fprintf(stderr, "Instructions Read: %zu\n", instruction_count);

	// Create image of the path Santa takes
	// Also count number of visited houses
	const size_t dimx = 2*X, dimy = 2*Y;
	unsigned char image_data[dimy][dimx][3];
	size_t num_houses_visited = 0, ic = 0;
	unsigned char black = 0;
	for (ix = 0; ix < X; ix++) {
		for (iy = 0; iy < Y; iy++) {
			int visit_count = house_grid[ix + iy * Y];
			if (visit_count > 0) {
				num_houses_visited++;
				unsigned char color[3];
				color[0] = (ix * visit_count) % 256;
				color[1] = (iy * visit_count) % 256;
				color[2] = (ix * iy * visit_count) % 256;
				for (ic = 0; ic < 3; ic ++) {
					image_data[2 * iy][2 * ix][ic] = color[ic];
					image_data[2 * iy + 1][2 * ix][ic] = color[ic];
					image_data[2 * iy][2 * ix + 1][ic] = color[ic];
					image_data[2 * iy + 1][2 * ix + 1][ic] = color[ic];
				}
			} else {
				for (ic = 0; ic < 3; ic ++) {
					image_data[2 * iy][2 * ix][ic] = black;
					image_data[2 * iy + 1][2 * ix][ic] = black;
					image_data[2 * iy][2 * ix + 1][ic] = black;
					image_data[2 * iy + 1][2 * ix + 1][ic] = black;
				}
			}
		}
	}
	FILE * imagefp = fopen("santa_path.ppm", "wb");
	fprintf(imagefp, "P6\n%zu %zu\n255\n", dimx, dimy); // header
	fwrite(image_data, sizeof(image_data), 1, imagefp);
	fclose(imagefp);

	fprintf(stderr, "There were %zu houses visited by Santa in spite of the inefficient routing.\n", num_houses_visited);

	// Cleanup
	free(command); // required by asprintf call
	free(house_grid);
	return 0;
}
