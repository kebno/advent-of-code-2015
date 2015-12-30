#ifndef __APPLE__
	error "Code only works for OS X"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#define COMMON_DIGEST_FOR_OPENSSL
#include <CommonCrypto/CommonDigest.h>

/* The challenge is to find the first MD5 hash (in hexadecimal) that begins with at least five zeros given the input that consists of a "secret key" appended with an integer. The "first" refers to the lowest positive number, which when appended (without zero padding) to the "secret key" results in an MD5 hash that begins with at least five zeros.

The solution here will compute the MD5 Message Digest for such inputs and will report the lowest positive number.
*/

int main(int argc, char * argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Please provide a secret key.\n");
		fprintf(stderr, "%s: [secret key]\n", argv[0]);
		return(1);
	}

	char * input = malloc(sizeof(unsigned char) * 20);
	size_t ii = 0, max_count = INT_MAX;

	for (ii = 0; ii < max_count; ii++) {
		int nchar = sprintf(input, "%s%0zu", argv[1], ii);
		// fprintf(stderr, "Input: %s\n", input);

		unsigned char md[CC_MD5_DIGEST_LENGTH];
		CC_MD5(input, nchar, md);

		// Convert 128 bit message digest to a 32 digit hexadecimal string.
		char mdstring[33];
		for (size_t i = 0; i < 16; i++) {
			sprintf(&mdstring[i*2], "%02x", (unsigned int)md[i]);
		}
		// fprintf(stderr, "Output: %s\n", mdstring);

		// If it finds it at the beginning of the hash
		if (mdstring == strstr(mdstring, "00000")) {
			printf("5 zeros Magic number is %zu\n", ii);
		}
		if (mdstring == strstr(mdstring, "000000")) {
			printf("6 zeros Magic number is %zu\n", ii);
			break;
		}
	}

	return(0);
}

/* Got the message digest to hexadecimal conversion code from:  http://www.askyb.com/cpp/openssl-md5-hashing-example-in-cpp/
My notes to explain the code and why I had not found success on my own:
I had been stumped for a while as I had not done the conversion from the 128 byte message digest (hash) to 32 hexadecimal digits.

The "%x" format specifier converts an unsigned int into a hexadecimal representation.

Each element of md (unsigned char) is 8 bits. It is converted to an unsigned int of 16 bits, done by "prepending zeros bits". That unsigned int may be converted to 4 hexadecimal digits. In this case, the first two hexadecimal digits would be 0 (char to unsigned int). The other two may be nonzero. The %02x format specifier causes the unisgned int to be interpreted as 2 digits, and that even zeros be printed.
Example of need for "%02x" as opposed to "%2x":
 '\a' is 7 as integer, and 07 as 2 digit hexadecimal.
 If printed with "%2x":  "7"
 If printed with "%02x": "07"
 The latter is correct as each 4 bits must be represented in the md5 message digest even if zero. */
