#include <stdio.h>
#include <string.h>
#import <libkern/OSAtomic.h>

char letters[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

const int NPARENTS = 3;
struct parent {
	int value;
};

int32_t shared_counter = 0;

int main(int argc, char * argv[]) {

	struct parent graph[NPARENTS][NPARENTS];

	for (int i = 0; i < NPARENTS; i++) {
		for (int j = 0; j < NPARENTS; j++) {
			graph[i][j].value = (i + 1) * (j + 1);
			OSAtomicIncrement32(&shared_counter);
		}
	}

	for (int i = 0; i < NPARENTS; i++) {
		for (int j = 0; j < NPARENTS; j++) {
			fprintf(stderr, "Parent value: %d\n", graph[i][j].value);
		}
	}
	fprintf(stderr, "Counter Result: %d\n", shared_counter);

	for (int i = 0; i < 26; i++) {
		fprintf(stdout, "%c\n", letters[i]);
	}

	for (int i = 0; i < 26; i++) {
		for (int j = 0; j < 26; j++) {
			fprintf(stdout, "%c%c\n", letters[i], letters[j]);
		}
	}

	return 0;
}
