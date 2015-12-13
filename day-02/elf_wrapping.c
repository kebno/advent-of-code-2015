#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main(int argc, char * argv[]){
	int dims[3];
	char c = 'a';
	int area_wrapping_paper = 0;
	int length_ribbon = 0;

	while(fscanf(stdin, "%d%c%d%c%d",&dims[0],&c,&dims[1],&c,&dims[2]) == 5) {
		//fprintf(stderr, "Read %d %d %d\n", dims[0],dims[1],dims[2]);
		
		/* Sort the three values. (thanks sorting-algorithms.com!) */
		size_t i, k;
		for (i = 1; i < 3; i++) {
			for (k = i; k > 0 && dims[k] < dims[k-1]; k--) {
				int tmp1 = dims[k-1];
				dims[k-1] = dims[k];
				dims[k] = tmp1;
			}
		}
		//fprintf(stderr, "Sorted Values: %d %d %d\n", dims[0],dims[1],dims[2]);

		int areas[3] = {0,0,0};
		areas[0] = dims[0] * dims[1];
		areas[1] = dims[0] * dims[2];
		areas[2] = dims[1] * dims[2];
		area_wrapping_paper += areas[0] * 3 + areas[1] * 2 + areas[2] * 2;

		length_ribbon += 2 * dims[0] + 2 * dims[1];
		length_ribbon += dims[0] * dims[1] * dims[2];
	}

	printf("Elves should order %d square feet of wrapping paper.\n", area_wrapping_paper);
	printf("Elves should order %d feet of ribbon.\n", length_ribbon);
	
	return 0;	
}
