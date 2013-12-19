// encoding.c
// encodes and decodes hex data as text
// compression ratio: 7 hex bytes <-> 8 text bytes

#include "encoding.h"
#include <stdlib.h>
#include <stdio.h>

// converts hex to text
// NOTE: assumes length is divisible by 7
unsigned char *encode(int length, unsigned char *data) {
	int num_blocks = length / 7;
	unsigned char *result = malloc(8 * num_blocks);

	// process blocks of length 7
	for (int i=0; i<num_blocks; i++) {
		for (int j=0; j<7; j++) {
			result[8*i + j + 1] = data[7*i + j] & 0x7f;
			result[8*i] += !!(data[7*i + j] & 0x80);
			result[8*i] <<= 1;
		}
		result[8*i] >>= 1;
	}

	return result;
}

// converts text to hex
// NOTE: assumes length is divisible by 8
unsigned char *decode(int length, unsigned char *data) {
	int num_blocks = length / 8;
	unsigned char *result = malloc(7 * num_blocks);

	// process blocks of length 8
	for (int i=0; i<num_blocks; i++) {
		for (int j=0; j<7; j++) {
			result[7*i + j] = ((data[8*i] >> (6 - j)) << 7) | data[8*i + j + 1];
		}
	}

	return result;
}

void test() {
	unsigned char test_data[] = { 0x00, 0x01, 0x02, 0x03, 0x80, 0x81, 0x82 };
	printf("test data: ");
	for (int i=0; i<7; i++) {
		printf("%x ", test_data[i]);
	}
	printf("\nencoded: ");
	unsigned char *encoded = encode(7, &test_data);
	for (int i=0; i<8; i++) {
		printf("%x ", encoded[i]);
	}
	printf("\ndecoded: ");
	unsigned char *decoded = decode(8, encoded);
	for (int i=0; i<7; i++) {
		printf("%x ", decoded[i]);
	}
	printf("\n");
}

#ifdef TEST_ENCODING
int main(int argc, char **argv) {
	test();
	exit(0);
}
#endif
