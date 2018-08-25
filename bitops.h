/*
 * A bit opearation utility header
 * Written by Tianpeng Chen z5176343 for COMP9319 Assignment 1
 * Inspired and huge thanks to our best tutor Haojie
 */

#include <stdio.h>

void addBit(char bitarray[], int k);
void getBitArray(char code[], char bitarray[]);
void writeBitArrayToFile(FILE** file, char bitarray[], int n);
void getCode8(char code[], char bitarray);
// int writeCharToFile(FILE* file, char c);
// int writeCodeToFile(FILE* file, char code[]);