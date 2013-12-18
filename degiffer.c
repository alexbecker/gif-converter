// degiffer.c
// extracts a file from a gif

#include <stdlib.h>
#include <stdio.h>

#define GIF_HEADER_SIZE 408
#define MAX_SUB_BLOCK_SIZE 255

int main(int argc, char **argv) {
	FILE *input = fopen(argv[1], "rb");
	FILE *output = fopen(argv[2], "wb");

	// skip the header
	fseek(input, GIF_HEADER_SIZE, SEEK_SET);

	unsigned char *buffer = malloc(MAX_SUB_BLOCK_SIZE);
	unsigned char sub_block_size;
	while ((sub_block_size = fgetc(input)) != 0x01) {
		fread(buffer, 1, sub_block_size, input);
		fwrite(buffer + 1, 1, sub_block_size - 1, output);
	}

	fclose(input);
	fclose(output);
	exit(0);
}
