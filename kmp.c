/*
 * A KMP-search implementation
 * Ref: https://www.geeksforgeeks.org/searching-for-patterns-set-2-kmp-algorithm/
 * Written by Tianpeng Chen z5176343 for COMP9319 assignment 1
 * Will return the last matched prefix to caller
 * since we are reading file byte-wise
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kmp.h"

// compute the longest-prefix-suffix array
void computeLPSArray(char const pattern[], int patlen, int* lps);

int kmp(char const pattern[], char const text[], char restTxt[]) {
	
	int patlen = strlen(pattern);
	int txtlen = strlen(text);
	int lps[patlen];
	int count = 0;
	int lastMatch = 0;

	computeLPSArray(pattern, patlen, lps);

	int i = 0;	// index for txt[]
	int j = 0; 	// index for pattern[]

	while (i < txtlen) {

		// printf("string[%d]: %c\n", i, text[i]);
		// printf("pattern[%d]: %c\n", j, pattern[j]);

		if (pattern[j] == text[i]) {
			lastMatch = i - j;
			j++;
			i++;
		}

		if (j == patlen) {
			// printf("Pattern found at pos %d\n", i - j);
			count++;
			// printf("-- kmp: lastMatch is %d --\n", lastMatch);
			j = lps[j - 1];
			lastMatch = i - j;
		} 

		// mismatch after j matches
		else if (i < txtlen && pattern[j] != text[i]) {

			// try to move j to j - 1
			if (j != 0) {
				j = lps[j - 1];
			} else {
			// move i
				i++;
			}
			lastMatch = i - j;
		}
	}
	// printf("Last match at %d\n", lastMatch);
	if (lastMatch != 0) {
		strcpy(restTxt, text + lastMatch);
	} else {
		memset(restTxt, 0, 513);
		// restTxt = "";
	}
	return count;
}


void computeLPSArray(char const pattern[], int patlen, int *lps) {
    // length of the previous longest prefix suffix
    int len = 0;
    // lps[0] is always 0
    lps[0] = 0;

    // calculates lps[i] for i in range(1, patlen - 1)
    int i = 1;

    while (i < patlen) {

    	if (pattern[i] == pattern[len]) {
    		len++;
    		lps[i] = len;
    		i++;
    	} else {
    		// similar to search strategy
    		if (len != 0) {
    			len = lps[len - 1];
    		} else {
    			lps[i] = 0;
    			i++;
    		}
    	}
    }
}

// int main() {
// 	char text[]		= "fox foo fo";
// 	// a : 1
// 	char patter[] 	= "fox";
// 	char rest[100] = {0};
// 	printf("%d\n", kmp(patter, text, rest));
// 	printf("rest: %s\n", rest);
// 	return 0;
// }