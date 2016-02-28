/* Try out parsing the input file for this test. */
#include <string.h>
#include <stdio.h>

#define NLEN 40
char * s_gets(char * line_buffer, int n);
void get_tokens(char * token_string, int len);

int main(const int argc, const char ** argv) {
	// char input[] = "ab AND ad -> ae, NOT ac -> ad";

	char input[NLEN];
	while (s_gets(input, NLEN)) {
		get_tokens(input, NLEN);
	}

	printf("Contents of the last line after token processing: \n'");
	for (size_t n = 0; n < NLEN; n++) {
		input[n] ? printf("%c", input[n]) : printf("\\0");
	}
	puts("'");

	return(0);
}

void get_tokens(char * token_string, int len) {
	int count = 0;
	printf("Parsing the input string '%s'\n", token_string);
	char * token = strtok(token_string, " ");
	while (token) {
		fprintf(stderr, "Length of token: %ld\n", strlen(token));
		puts(token);
		token = strtok(NULL, " ");
		count++;
	}
	printf("There were %d tokens found\n\n", count);
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
