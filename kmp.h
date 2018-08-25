/*
 * A KMP-search implementation header
 * Ref: https://www.geeksforgeeks.org/searching-for-patterns-set-2-kmp-algorithm/
 * Written by Tianpeng Chen z5176343 for COMP9319 assignment 1
 * 
 * Since we are searching in a buffer rather than the whole file,
 * if mismatch happens, this implementation will also keep the last
 * possible prefix in the restTxt string.
 * In the next iteration, it will concatenate with the next bytes from the file.

 * For example, search fox in string "foo foo fo" (next buffer is "x foo fox")
 * The 1st iteration will return count 0 and restTxt "fo".
 * In the 2nd iteration, the text to be search is "fox foo fox", thus count will be 2.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int kmp(char const pattern[], char const text[], char restTxt[]);