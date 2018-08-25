/*
 * a MinHeap ADT Header
 * Ref: www.geeksforgeeks.org/greedy-algorithms-set-3-huffman-coding
 * Written by Tianpeng Chen z5176343 for COMP9319 assignment 1
 * Destroy method added
 * Table transfer function added
 */

// a huffman tree node structure

typedef struct MinHeapNode {
	unsigned char data;
	unsigned freq;
	struct MinHeapNode *left, *right;
	int valid;	// 1 for data node, 0 for internal node
} MinHeapNode;

// a huffman tree structure
typedef struct MinHeapST {
	unsigned size;
	unsigned capacity;
	MinHeapNode **array;
} MinHeap;

// code table
unsigned char* table[256];

// to destroy
MinHeap* globalMinHeapPtr;

MinHeapNode* newNode(unsigned char data, unsigned freq, int valid);
MinHeap* createMinHeap(unsigned capacity);
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b);
void minHeapify(MinHeap* minHeap, int index);
int isSizeOne(MinHeap* minHeap);
MinHeapNode* getMin(MinHeap* minHeap);
void insertMinHeap(MinHeap* minHeap, MinHeapNode* minHeapNode);
void buildMinHeap(MinHeap* minHeap);
void printArray(unsigned char array[], unsigned n);
int isLeaf(MinHeapNode* root);
MinHeap* createAndBuildMinHeap(unsigned char data[], unsigned freq[], int size);
MinHeapNode* buildHuffmanTree(unsigned char data[], unsigned freq[], int size);
void printHuffman(MinHeapNode* root, unsigned char arrry[], unsigned top);
// expose huffmantree pointer to outside
unsigned char** huffmanCodes(MinHeapNode** tree, unsigned char data[], unsigned freq[], int size);
// int* getCode(unsigned char token);
// HuffmanTable* initTable(int size);
void transferToTable(MinHeapNode* root, unsigned char array[], unsigned top, unsigned char* table[]);
void printTable(unsigned char* table[]);
void destroyMinHeap();
void destroyHuffmanTree(MinHeapNode* tree);
// expose root pointer outside, move this function to huffman.c
// void getunsigned char(MinHeapNode* root, unsigned char* code[], int* cur, int* n, unsigned char* decoded[], unsigned char* rest[]);