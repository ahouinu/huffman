/*
 * COMP9319 18s2 Assignment 1 - Huffman
 * Author: Tianpeng Chen z5176343
 * Simple autotest passed: 22/08/18
 * Memory check passed
 * Memory optimised
 * Refactor code, remove unnecessary comments and codes
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minheap.h"
#include "bitops.h"
#include "kmp.h"

#define BUFFER_SIZE 256
#define REST_BUFFER_SIZE 32


int printUsageAndExit(char const *argv[]) {

	printf("Usage:\n\t%s -e [input-src-path] [output-src-path] or\n"
        		, argv[0]);
        printf("\t%s -d [input-src-path] [output-src-path] or\n"
        	, argv[0]);
        printf("\t%s -s [query-string] [file-path]\n"
        	, argv[0]);

        return 1;
}


int charDecoder(MinHeapNode* root, char code[], int n, char ans[], char rest[], int* nbOfCharDecoded) {
    
    MinHeapNode* curr = root;
    int i;
    
    int nbOfChar = 0;
    int bitCount = 0;
    
    for (i = 0; i < n; i++) {
    
        if (code[i] == '0') {
            curr = curr->left;
        } else {
            curr = curr->right;
        }

        // reached left
        if (isLeaf(curr)) {
    
            ans[nbOfChar++] = curr->data;
            curr = root;
            bitCount = i + 1;
        }
    }
    memset(rest, 0, REST_BUFFER_SIZE);
    strcpy(rest, code + bitCount);
    ans[nbOfChar] = '\0';
    
    *nbOfCharDecoded = nbOfChar;
    
    return n - bitCount;

}

int encode(char const *inputFilePath, char const *outputFilePath) {

	FILE *inptr = fopen(inputFilePath, "r");
    FILE *outptr = fopen(outputFilePath, "wb");

    MinHeapNode* root = (MinHeapNode* )malloc(sizeof(MinHeapNode));

    if (inptr == NULL) {
        printf("Input file not exist!\n");
        return 1;
    }

    unsigned freq[256];
    int i;
    
    // init freq array
    for (i = 0; i < 256; i++) {
        freq[i] = 0;
    }    

    int code;
    int count = 0;
    int letterCount = 0;
    while ((code = fgetc(inptr)) != EOF) {
        // if (code == 0) continue;
        freq[code]++;
        count++;
        if (freq[code] == 1) {
            letterCount++;
        }
    }
    
    unsigned char *data = (unsigned char *)malloc(letterCount * sizeof(unsigned char));
    unsigned *hufFreq = (unsigned *)malloc(letterCount * sizeof(unsigned));
    int j = 0;
    for (i = 0; i < 256; i++) {
        if (freq[i] == 0) continue;
        data[j] = (unsigned char) i;
        hufFreq[j] = freq[i];
        j++;
    }

    // encoding
    // int compressedLength = 0;
    // generate freq table from minheap.h
    unsigned char **table = huffmanCodes(&root, data, hufFreq, j);

    // int k;
    // for (k = 0; k < letterCount; k++) {
        // int index = (int) data[k];
        // compressedLength += (int) strlen(table[index]) * hufFreq[k];
    // }
    free(data);
    free(hufFreq);

    // write freq table to header

    fwrite(freq, sizeof(int), 256, outptr);
    rewind(inptr);

    int ascii = 0;
    char buffer[9] = "\0";
    // char rest[9] on CSE machine, may cause memory coruption with
    // buffer, which fails test3
    char rest[16] = "\0";
    char bitarray = 0;
    int offset = 0;
    int loopCount = 0;
    int bitCount = 0;
    char huffmancode[256] = "\0";
    int len = 0;
    while (1) {

        if (strlen(buffer) > 8) break;
        // if buffer is full, writes out buffer
        if (strlen(buffer) == 8) {
            getBitArray(buffer, &bitarray);
            fwrite(&bitarray, sizeof(char), 1, outptr);
            // reset bitarray
            // memset(&bitarray, 0, 1);
            bitarray = 0;
            bitCount += 8;
            offset = 0;
            memset(buffer, 0, 8);
            continue;
        }


        // if rest is not empty
        if (strlen(rest) != 0) {
            strncpy(huffmancode, rest, strlen(rest));
            memset(rest, 0, 16);
        } else {
        // read new char
            ascii = fgetc(inptr);

            if (ascii == EOF) {
                if (strlen(buffer) != 0) {
                    // make up with trailing 0's (ASCII 48)
                    memset(buffer + strlen(buffer), 48, 8 - strlen(buffer));
                    getBitArray(buffer, &bitarray);
                    fwrite(&bitarray, sizeof(char), 1, outptr);
                    // reset bitarray
                    // memset(&bitarray, 0, 1);
                    bitarray = 0;
                }
                break;
            }
            strcpy(huffmancode, table[ascii]);
            len = strlen(huffmancode);
        }

        // every time writes out 8 bits / 1 byte
        
        // read 8 bits into buffer
        // if len(code) > 8
        // read the first 8 bits into buffer, the rest into rest
        if (len + offset > 8) {
            int bufferRestCapacity = 8 - offset;
            strncpy(buffer + offset, huffmancode, bufferRestCapacity);
            strncpy(rest, huffmancode + 8 - offset, len);
            offset = 8;
            len -= bufferRestCapacity;
            continue;
        }
        // if len(code) < 8
        // read the next token, set offset = len
        else if (len + offset <= 8) {
            strncpy(buffer + offset, huffmancode, len);
            offset += len;
            continue;
        }

    }

    // clean up
    fclose(inptr);
    fclose(outptr);

    destroyHuffmanTree(root);
    // free(root);

    for (i = 0; i < 256; i++) {
        if (table[i]) {
            free(table[i]);
        }
    }

    return 0;

}


int decode(char const *inputFile, char const *outputFile) {

    FILE* inptr = fopen(inputFile, "rb");
    FILE* outptr = fopen(outputFile, "wb");

    MinHeapNode* root = (MinHeapNode*)malloc(sizeof(MinHeapNode));

    if (inptr == NULL) {
        printf("Inputr file not exist!\n");
        return 1;
    }

    // read header
    unsigned freqArray[256] = {0};
    unsigned char tokenArray[256] = {0};
    int i;
    int j = 0; // number of tokens read
    int freq = 0;
    int nbOfChar = 0;
    for (i = 0; i < 256; i++) {
        if (freq == EOF) {
            printf("Not a valid header! Terminating...\n");
            return 1;
        }
        fread(&freq, sizeof(int), 1, inptr);
        if (freq == 0) continue;
        freqArray[j] = freq;
        tokenArray[j] = (char) i;
        nbOfChar += freq;
        j++;
    }

    // build huffman tree
    unsigned char **table = huffmanCodes(&root, tokenArray, freqArray, j);

    // ready to decode file
    int currentBufferSize = 8;
    char bitarray = 0; // 8 bits
    char code[8 + 1] = {0};
    int charCounter = 0;
    char decoded[9] = {0};
    char rest[REST_BUFFER_SIZE] = {0}; 
    int undecoded = 0;

    while (charCounter < nbOfChar) {
        // read 1 byte (8 bits)
        fread(&bitarray, sizeof(char), 1, inptr);
        getCode8(code, bitarray);

        int nbOfCharDecoded = 0;
        undecoded = charDecoder(root, code, currentBufferSize, decoded, rest, &nbOfCharDecoded);
        charCounter += nbOfCharDecoded;
        
        int overWritten = charCounter - nbOfChar;
        if (overWritten > 0) {
            int outputLength = nbOfCharDecoded - overWritten;
            char *tmp = (char *)malloc(
                (outputLength + 1) * sizeof(char));
            memset(tmp, 0, outputLength + 1);
            strncpy(tmp, decoded, outputLength);
            tmp[outputLength] = 0;
            fwrite(tmp, sizeof(char), outputLength, outptr);
            free(tmp);
            break;
        } else {
                // write to file
                int outputLength = nbOfCharDecoded;
                fwrite(decoded, sizeof(char), outputLength, outptr);
        }   

        while (strlen(rest) != 0) {
            // read another 8 bits
            char newCode[8 + 1] = {0};
            fread(&bitarray, sizeof(char), 1, inptr);
            getCode8(newCode, bitarray);
            currentBufferSize += strlen(rest);
            
            char* tmp = (char *)malloc(
                (currentBufferSize + 1 + 8) * sizeof(char));
            char newRest[REST_BUFFER_SIZE] = {0};
            memset(tmp, 0, currentBufferSize + 1 + 8);
            strcpy(tmp, rest);
            strncat(tmp, newCode, strlen(newCode));
            
            int nbOfCharDecoded = 0;
            undecoded = charDecoder(root, tmp, (int)strlen(tmp), decoded, newRest, &nbOfCharDecoded);

            charCounter += nbOfCharDecoded;
            strcpy(rest, newRest);
            currentBufferSize = undecoded;

            overWritten = charCounter - nbOfChar;
            if (overWritten > 0) {
                int outputLength = nbOfCharDecoded - overWritten;

                char *tmp2 = (char *)malloc(
                    (outputLength + 1) * sizeof(char));
                memset(tmp2, 0, outputLength + 1);
                strncpy(tmp2, decoded, outputLength);
                tmp2[outputLength] = 0;

                fwrite(tmp2, sizeof(char), outputLength, outptr);
                free(tmp2);
                break;
            } else {
                // write to file
                int outputLength = nbOfCharDecoded;
                fwrite(decoded, sizeof(char), outputLength, outptr);
            }
            // a dummy final memory check
            if (tmp) free(tmp);
        }
        // reset buffer size
        currentBufferSize = 8;
    }

    // clean up
    fclose(inptr);
    fclose(outptr);

    destroyHuffmanTree(root);
    // free(root);

    for (i = 0; i < 256; i++) {
        if (table[i]) {
            free(table[i]);
        }
    }

	return 0;
}


int search(char const *pattern, char const *inputFile) {
    // consider doing kmp
    // If alphabet is large, boyer-moore may have better performance
    // But in my case, kmp works just fine and I really don't have time
    // to try BM...

    // This section uses a adapted KMP search algorithm implemented in
    // kmp.c, which also saves the last possible prefix to a string.

    int count = 0;
    FILE* file = fopen(inputFile, "rb");

    MinHeapNode* root = (MinHeapNode*)malloc(sizeof(MinHeapNode));

    if (file == NULL) {
        printf("Input file not exist!\n");
        return 1;
    }

    // read header
    unsigned freqArray[256] = {0};
    unsigned char tokenArray[256] = {0};
    unsigned freqTable[256] = {0};
    int i;
    int j = 0; // number of tokens read
    int freq = 0;
    int nbOfChar = 0;
    for (i = 0; i < 256; i++) {
        if (freq == EOF) {
            printf("Not a valid header! Terminating...\n");
            return 1;
        }
        fread(&freq, sizeof(int), 1, file);
        if (freq == 0) continue;
        freqArray[j] = freq;
        freqTable[i] = freq;
        tokenArray[j] = (char) i;
        nbOfChar += freq;
        j++;
    }
    
    // if pattern is a single char
    // return char freq directly in freq array
    if (strlen(pattern) == 1) {
        int index = (int) pattern[0];
        count = freqTable[index];
        printf("%d\n", count);
        return 0;
    }

    // if pattern contains char which is not in the freq table
    // it means this pattern won't occur in the file
    // return 0
    for (i = 0; i < strlen(pattern); i++) {
        int index = (int) pattern[i];
        if (freqTable[index] == 0) {
            printf("%d\n", 0);
            return 0;
        }
    }

    // else, we need to decode the file iteratively into a buffer
    // and search pattern in the buffer

    // build huffman tree
    unsigned char **table = huffmanCodes(&root, tokenArray, freqArray, j);

    // calculate buffer size
    // first we need the encoded pattern size
    int patlen = strlen(pattern);

    // buffer size will be patlen * 2 bytes

    int charCounter = 0;
    int bufferSize = patlen * 2;        // bytes
    int currentBufferSize = 8;          // how many bits of code to read each time
    int nbOfCharRead = 0;
    char buffer[512 + 1] = {0};         // max patlen is 256 bytes
    char restTxt[512 + 1] = {0};
    char code[32] = {0};    
    char rest[REST_BUFFER_SIZE];
    char decoded[8 + 1] = {0};
    char bitarray = 0;
    char lastChar = 0;
    int bufferOffset = 0;
    int patternOffset = 0;
    int undecoded = 0;

    // This decoding section is almost copied directly from the previous
    // function decode(), which is dummy...
    // But it works just fine...

    // decode file byte-wise and do kmp search
    // every time keep the last unmatched few bytes
    while (charCounter < nbOfChar) {
        // read into buffer
        while (nbOfCharRead < bufferSize) {
            fread(&bitarray, sizeof(char), 1, file);
            getCode8(code, bitarray);

            int nbOfCharDecoded = 0;

            undecoded = charDecoder(root, code, currentBufferSize, decoded, rest, &nbOfCharDecoded);
            charCounter += nbOfCharDecoded;
            nbOfCharRead += nbOfCharDecoded;

            strcpy(buffer + bufferOffset, decoded);
            bufferOffset += nbOfCharDecoded;

            int overWritten = charCounter - nbOfChar;
            if (overWritten > 0) {
                int outputLength = nbOfCharDecoded - overWritten;
                bufferOffset -= overWritten;
                buffer[bufferOffset] = 0;

                // write to buffer
                // strncpy(buffer + bufferOffset, decoded, outputLength);
                break;
            } 

            while (strlen(rest) != 0) {

                char newCode[8 + 1] = {0};
                fread(&bitarray, sizeof(char), 1, file);
                getCode8(newCode, bitarray);

                currentBufferSize += strlen(rest);

                char tmp[REST_BUFFER_SIZE] = {0};
                char newRest[REST_BUFFER_SIZE] = {0};

                strcpy(tmp, rest);
                strncat(tmp, newCode, strlen(newCode));

                int nbOfCharDecoded = 0;
                undecoded = charDecoder(root, tmp, (int)strlen(tmp), decoded, newRest, &nbOfCharDecoded);

                charCounter += nbOfCharDecoded;
                nbOfCharRead += nbOfCharDecoded;
                strcpy(rest, newRest);
                currentBufferSize = undecoded;

                strcpy(buffer + bufferOffset, decoded);
                bufferOffset += nbOfCharDecoded;


                overWritten = charCounter - nbOfChar;
                if (overWritten > 0) {
                    int outputLength = nbOfCharDecoded - overWritten;
                    bufferOffset -= overWritten;
                    buffer[bufferOffset] = 0;

                    // write to buffer
                    strncpy(buffer + bufferOffset, decoded, outputLength);
                    break;
                }

                // search pattern in buffer
                if (nbOfCharRead >= bufferSize) {
                    buffer[bufferOffset] = 0;
                    count += kmp(pattern, buffer, restTxt);

                    // reset buffer, etc
                    memset(buffer, 0, 513);
                    bufferOffset = 0;
                    nbOfCharRead = 0;
                    // copy restTxt to buffer
                    strcpy(buffer, restTxt);
                    bufferOffset = strlen(buffer);
                }
            }
        }

        // search pattern in buffer
        buffer[bufferOffset] = 0;
        count += kmp(pattern, buffer, restTxt);
        // reset buffer, etc
        memset(buffer, 0, 513);
        bufferOffset = 0;
        nbOfCharRead = 0;
        // copy restTxt to buffer
        strcpy(buffer, restTxt);
        bufferOffset = strlen(buffer);
    }

    // print result to STDOUT
    printf("%d\n", count);

    // clean up

    for (i = 0; i < 256; i++) {
        if (table[i]) {
            free(table[i]);
        }
    }

    destroyHuffmanTree(root);
    

    fclose(file);

	return 0;
}

int main(int argc, char const *argv[]) {

    if (argc != 4) {
        return printUsageAndExit(argv);
    }

    if (strcmp(argv[1], "-e") == 0) {
    	// printf("encoding %s into %s\n", argv[2], argv[3]);
    	encode(argv[2], argv[3]);
    } else if (strcmp(argv[1], "-d") == 0) {
    	// printf("decoding %s into %s\n", argv[2], argv[3]);
    	decode(argv[2], argv[3]);
    } else if (strcmp(argv[1], "-s") == 0) {
    	// printf("searching %s in %s\n", argv[2], argv[3]);
    	search(argv[2], argv[3]);
    } else {
    	return printUsageAndExit(argv);
    }

	return 0;

}
