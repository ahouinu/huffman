/*
 * A bit opearation utility implementation
 * Written by Tianpeng Chen z5176343 for COMP9319 Assignment 1
 * Inspired and huge thanks to our best tutor Haojie
 */

#include "bitops.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void addBit(char bitarray[], int k) {
	int i = k / 8;
	int j = k % 8;
	bitarray[i] = ((0x80 >> j) | bitarray[i]);
}

void getBitArray(char code[], char bitarray[]) {
	// reset bitarray
	memset(bitarray, 0, 1);
	// printf("bitops: bitarray length is %ld\n", strlen(bitarray));
	int len = strlen(code);
	int i;
	for (i = 0; i < len; i++) {
		if (code[i] == '1') {
			// printf("adding bit 1 to pos %d\n", i);
			addBit(bitarray, i);
		}
	}
	// printf("bitops: bitarray length is %ld\n", strlen(bitarray));
    // printf("bitops: bitarray is: %s\n", bitarray);
}

void getCode8(char code[], char bitarray) {
	int i; // assume 8 bits
	char ch;
	for (i = 0; i < 8; i++) {
		char mask = 0x80 >> i;
		// printf("Mask: %c\n", mask);
		ch = (mask & bitarray);
		// printf("Char: %c\n", ch);
		// if the i-th bit is 1
		if (ch == mask) {
			code[i] = '1';
		} else {
			code[i] = '0';
		}
	}
	code[8] = 0;
	// printf("bitops: %s\n", code);

}

void writeBitArrayToFile(FILE** file, char bitarray[], int n) {

	assert(file != NULL);

	fwrite(bitarray, sizeof(char), n, *file);
}

// int main(int argc, char const *argv[])
// {
// 	char code[32] = "00101110000011110010110111001111";
// 	// printf("code: %s\n", code);
// 	char bitarray[4] = {0};
// 	FILE* file = fopen("bit_tmp", "wb");
// 	getBitArray(code, bitarray);
// 	fwrite(bitarray, sizeof(char), 4, file);
// 	fclose(file);
// 	// printf("bitarray: %s\n", bitarray);
// 	// int i;
// 	// for (i = 0; i < strlen(bitarray); i++) {
// 	// 	printf("%c\n", bitarray[i]);
// 	// }
// 	return 0;
// }