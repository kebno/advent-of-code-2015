/* Try out parsing the input file for this test. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define NLEN 25
char * s_gets(char * line_buffer, int n);
size_t get_tokens(char ** token_array, int len, char * token_string);

void fill_node();

int main(const int argc, const char ** argv) {
	char input[NLEN];
	while (s_gets(input, NLEN)) {
		char * * token_array = NULL;
		size_t token_count = get_tokens(token_array, NLEN, input);

		printf("Lengths of %zu tokens: \n", token_count);
		fflush(stdout);
		for (size_t i = 0; i < token_count; i++) {
			printf("%lu ", strlen(token_array[i]));
		}
		printf("\n");
	}

	printf("Contents of the last line after token processing: \n'");
	for (size_t n = 0; n < NLEN; n++) {
		input[n] ? printf("%c", input[n]) : printf("\\0");
	}
	puts("'");

	return(0);
}

void fill_node(void) {

}


size_t get_tokens(char ** token_array, int len, char * token_string) {
	printf("Parsing the input string '%s'\n", token_string);
	/* make a copy for later use as strtok replaces separator with '\0' */
	char * token_string_copy = strdup(token_string);

	/* Count the number of tokens in this string */
	size_t token_count = 0;
	char * token = strtok(token_string, " ");
	while (token) {
		// puts(token);
		token_count++;
		token = strtok(NULL, " ");
	}
	// printf("There were %zu tokens found\n\n", token_count);

	/* Store each token in a character array */
	token_array = malloc(token_count * sizeof(char *));
	token_count = 0;
	token = strtok(token_string_copy, " ");
	while (token) {
		token_array[token_count] = strdup(token);

		printf("%s\n", token_array[token_count]);
		fprintf(stderr, "Length of token: %ld\n", strlen(token_array[token_count]));

		/* get next token */
		token = strtok(NULL, " ");
		token_count++;
	}
	printf("There were %zu tokens found\n\n", token_count);
	return token_count;
}

char * s_gets(char * line_buffer, int n) {
	char * return_value;
	char * find;

	return_value = fgets(line_buffer, n, stdin);
	/* look for newline, replace it with \0, and discard the rest of the input */
	if (return_value) {
		find = strchr(line_buffer, '\n');
		if (find) {
			*find = '\0';
		} else {
			while ((getchar() != EOF) && (getchar() != '\n'))
				continue;  // dispose of the rest of line
		}
	}
	return return_value;
}
