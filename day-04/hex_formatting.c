#include <stdio.h>

int main(int argc, char const *argv[]) {
	unsigned int num = '\a';
	fprintf(stderr, "Num as integer: %d\n", num);
	fprintf(stderr, "Num as 4 digit hex: %4x\n", num);
	fprintf(stderr, "Num as 2 digit hex: %2x\n", num);
	fprintf(stderr, "Num as 04 digit hex: %04x\n", num);
	fprintf(stderr, "Num as 02 digit hex: %02x\n", num);
	return 0;
}
