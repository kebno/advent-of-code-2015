#ifndef __APPLE__
	error "Code only works for OS X (b/c of CommonCrypto, but you could use the openssl MD5 function instead, just do a swap)"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#define COMMON_DIGEST_FOR_OPENSSL
#include <CommonCrypto/CommonDigest.h>
#include <dispatch/dispatch.h>

/* The challenge is to find the first MD5 hash (in hexadecimal) that begins with at least five zeros given the input that consists of a "secret key" appended with an integer. The "first" refers to the lowest positive number, which when appended (without zero padding) to the "secret key" results in an MD5 hash that begins with at least five zeros.

The solution here will compute the MD5 Message Digest for such inputs and will report the lowest positive number.

But now with LIBDISPATCH ("so shiny and new!")  ("way of the future!")
*/

/*
 * The print_serial queue is used to serialize printing.
 */
static dispatch_queue_t printer_q = NULL;
static dispatch_queue_t compute_q = NULL;

int main(int argc, char * argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Please provide a secret key.\n");
		fprintf(stderr, "%s: [secret key]\n", argv[0]);
		return(1);
	}

	compute_q = dispatch_queue_create("Compute Queue", NULL);
	printer_q = dispatch_queue_create("Print Queue", NULL);

	char * input = malloc(sizeof(char) * 40);
	size_t max_count = ULONG_MAX;

	// Run Checking in blocks of size 1e6
	int block_size = 1e6;
	int nblocks = max_count / block_size;
	for (size_t block_num = 0; block_num < nblocks; block_num++) {
		dispatch_async(compute_q, ^{
			for (size_t ii = 1e6 * block_num; ii < (1e6 * (block_num + 1)); ii++) {
				int nchar = sprintf(input, "%s%0zu", argv[1], ii);

				unsigned char md[CC_MD5_DIGEST_LENGTH];
				CC_MD5(input, nchar, md);

				// Convert 128 bit message digest to a 32 digit hexadecimal string.
				char mdstring[33];
				for (size_t i = 0; i < 16; i++) {
					sprintf(&mdstring[i*2], "%02x", (unsigned int)md[i]);
				}

				int zero_count = 0;
				for (size_t i = 0; i < 6; i++) {
					if (mdstring[i] == '0')
						zero_count++;
				}

				if (zero_count == 6) {
					dispatch_sync(printer_q, ^{ printf("\n6 zeros Magic number is %zu\n", ii); });
					exit(0);
				} else if (zero_count == 5) {
					dispatch_async(printer_q, ^{
						fprintf(stderr, ".");
					});
				}
			}
		});

		if (!(block_num % 8))
			dispatch_sync(compute_q, ^{
				fprintf(stderr, "\n4 Blocks Completed, On Block %zu\n", block_num);
				fprintf(stderr, "Max tested value is at least %d\n", (int)((block_num + 1) * 1e6));
			});
	}

	return(0);
}

/* Got the message digest to hexadecimal conversion code from:  http://www.askyb.com/cpp/openssl-md5-hashing-example-in-cpp/
My notes about it are the in the non-libdispatch version of this program. */
