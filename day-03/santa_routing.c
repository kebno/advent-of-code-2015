#include <stdio.h>
#include <stdlib.h>

/*
 * Use the input file as instructions for traversing a 2D grid.
 * Create an image to show the path taken for these instructions.
 */

int main(int argc, char * argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Error: %s, provide an input filename.\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Get number of characters in the input file */
	// form command
	char * command = NULL;
	asprintf(&command, "/usr/bin/wc -m %s", argv[1]);
	printf("The command to be run: %s\n", command);

	// run the command
	FILE * fp;
	if ((fp = popen(command, "r")) == NULL) {
		fprintf(stderr, "Failed to run command: %s\n", command);
		exit(EXIT_FAILURE);
	}

	// read the command result (something like:  "1024 input.txt")
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

	// Ask if it is okay to produce the potentially large image
	printf("This program will create an image of about %.0f MB\n", 4 * num_characters * num_characters * 1e-6);
	printf("Are you okay with that? (y/n)\n");
	if (getc(stdin) == 'n') {
		printf("Okay. Exiting...\n");
		exit(0);
	}

	// Setup and create the grid
	const size_t X = num_characters, Y = num_characters;
	size_t ix, iy;
	


	// Cleanup
	free(command); // required by asprintf call

	return 0;
}
