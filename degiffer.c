// degiffer.c
// extracts a file from a gif

#include "encoding.h"
#include <stdlib.h>
#include <stdio.h>

#define GIF_HEADER_SIZE 408
#define MAX_SUB_BLOCK_SIZE 255

// value to indicate beginning of file padding
#define PAD_HEADER_SIZE 7
unsigned char pad_header[] = { 0xde, 0xad, 0xbe, 0xef, 0xa5, 0x5e, 0x55 };

int main(int argc, char **argv) {
	FILE *input = fopen(argv[1], "rb");
	FILE *output = fopen(argv[2], "wb");

	// skip the header
	fseek(input, GIF_HEADER_SIZE, SEEK_SET);

	unsigned char *input_buffer = malloc(MAX_SUB_BLOCK_SIZE), *output_buffer;
	unsigned char sub_block_size;
	int bytes_written;
	while (1) {
		sub_block_size = fgetc(input);
		fread(input_buffer, 1, sub_block_size, input);
		output_buffer = decode(sub_block_size - 1, input_buffer + 1);
		bytes_written = 7 * (sub_block_size - 1) / 8;

		// check to see if padding has started
		for (int i=0; i <= bytes_written - PAD_HEADER_SIZE; i++) {
			for (int j=0; j<PAD_HEADER_SIZE; j++) {
				if (output_buffer[i + j] != pad_header[j]) {
					goto next_iteration;
				}
			}
			// at padding, write and exit
			if (i > 0) {
				fwrite(output_buffer, 1, i, output);
			}
			goto cleanup;

			next_iteration:
				;
		}

		// not yet at padding
		fwrite(output_buffer, 1, bytes_written, output);
	}

	cleanup:
		fclose(input);
		fclose(output);
		exit(0);
}
