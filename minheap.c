/*
 * a MinHeap ADT implementation
 * Ref: www.geeksforgeeks.org/greedy-algorithms-set-3-huffman-coding
 * Written by Tianpeng Chen z5176343 for COMP9319 assignment 1
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minheap.h"
#define MAX_TREE_HEIGHT 100


MinHeapNode* newNode(unsigned char data, unsigned freq, int valid) {

	MinHeapNode* node = 
	(MinHeapNode*)malloc(sizeof(MinHeapNode));

	node->left = node->right = NULL;
	node->data = data;
	node->freq = freq;
	node->valid = valid;

	return node;
}

MinHeap* createMinHeap(unsigned capacity) {

	MinHeap* minHeap 
		= (MinHeap*)malloc(sizeof(MinHeap));

	minHeap->size = 0;

	minHeap->capacity = capacity;

	minHeap->array 
		= (MinHeapNode**)malloc(minHeap->capacity * sizeof(MinHeapNode*));
	// printf("After created, minheap at %p\n\n", minHeap);
	// printf("After created, &minheap at %p\n\n", &minHeap);
	return minHeap;
}

void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
	
	MinHeapNode* tmp = *a;
	
	*a = *b;
	*b = tmp;

}

void minHeapify(MinHeap* minHeap, int index) {
	
	int smallest = index;
	int left = 2 * index + 1;
	int right = 2 * index + 2;

	//TODO: figure out this section!

	if (left < minHeap->size && 
		minHeap->array[left]->freq < minHeap->array[smallest]->freq) {

		smallest = left;

	}

	if (right < minHeap->size && 
		minHeap->array[right]->freq < minHeap->array[smallest]->freq) {
		
		smallest = right;
	}

	if (smallest != index) {
		
		swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[index]);
		
		minHeapify(minHeap, smallest);

	}
}

int isSizeOne(MinHeap* minHeap) {
	
	return (minHeap->size == 1);

}

MinHeapNode* getMin(MinHeap* minHeap) {

	MinHeapNode* min = minHeap->array[0];
	minHeap->array[0]
		 = minHeap->array[minHeap->size - 1];

	--minHeap->size;
	minHeapify(minHeap, 0);

	return min;
}

void insertMinHeap(MinHeap* minHeap, MinHeapNode* minHeapNode) {

	++minHeap->size;
	int i = minHeap->size - 1;

	while (i && minHeapNode->freq 
		< minHeap->array[(i - 1) / 2]->freq) {

		minHeap->array[i] = minHeap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}

	minHeap->array[i] = minHeapNode;
}

void buildMinHeap(MinHeap* minHeap) {
	
	int n = minHeap->size - 1;
	int i;

	for (i = (n - 1) / 2; i >= 0; --i) {
		minHeapify(minHeap, i);
	}
	// printf("After built, minheap at %p\n\n", minHeap);
	// printf("After built, &minheap at %p\n\n", &minHeap);
}

void printArray(unsigned char array[], unsigned n) {
	
	int i;
	for (i = 0; i < n; ++i) {
		printf("%c", array[i]);
	}
	printf("\n");
}

int isLeaf(MinHeapNode* root) {
	
	return !(root->left) && !(root->right);

}

MinHeap* createAndBuildMinHeap(unsigned char data[], unsigned freq[], int size) {

	MinHeap* minHeap = createMinHeap(size);

	int i;
	for (i = 0; i < size; ++i) {
		// printf("Hf data:%c\t freq:%d\n", data[i], freq[i]);
		minHeap->array[i] = newNode(data[i], freq[i], 1);
	}

	minHeap->size = size;
	buildMinHeap(minHeap);
	globalMinHeapPtr = minHeap;
	return minHeap;
	
}

MinHeapNode* buildHuffmanTree(unsigned char data[], unsigned freq[], int size) {
	
	MinHeapNode *left, *right, *top;

	// 1. Create a min heap of capacity == size.

	MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);
	// minHeap = createAndBuildMinHeap(data, freq, size);

	while (!isSizeOne(minHeap)) {

		// 2. Get 2 minimum freq items from minHeap

		left = getMin(minHeap);
		right = getMin(minHeap);

		// 3. Create a new internal node wtih freq = left->freq + right->freq
		// put -1 sign for this internal node (means not used)

		top = newNode(-1, left->freq + right->freq, 0);

		top->left = left;
		top->right = right;

		insertMinHeap(minHeap, top);
	}

	// 4. The remaining node is the root node and the tree is complete
	return getMin(minHeap);
}

void printHuffman(MinHeapNode* root, unsigned char array[], unsigned top) {


	// unsigned int index = (unsigned) root->data;
	// printf("???%u\n", root->data);
	// printf("root at %p\n", root);
	// printf("ascii: %u\n", index);
	// printf("valid?: %d\n", root->valid);
	// printf("Now array is %s\n", array);
	// printf("top: %d\n\n", top);
	// recursively assign 0 to left children

	if (root->left) {
		array[top] = '0';
		printHuffman(root->left, array, top + 1);
	}

	// recursively assign 1 to right children

	if (root->right) {
		array[top] = '1';
		printHuffman(root->right, array, top + 1);
	}

	// If isLeaf, then it contains one of the input unsigned characters, 
	// printout this unsigned character and its code from array[]

	if (isLeaf(root)) {
		printf("%c: ", root->data);
		printf("\t%u: ", (unsigned)root->data);
		printArray(array, top);
	}
	
}

/*
HuffmanTable* initTable(int size) {

	HuffmanTable* table = (HuffmanTable*)malloc(size * sizeof(HuffmanTable));
	printf("Table initialized... of size %d\n", size);
	int i;
	for (i = 0; i < size; i++) {
		printf("%c")
	}

	return table;
}
*/

// BUG!
// unsigned char* arrayToString(int src[], int n) {
	
// 	int i;
// 	unsigned char* res = (unsigned char*)malloc(n * sizeof(unsigned char));
// 	// printf("Code: ");
// 	for (i = 0; i < n; ++i) {
// 		res[i] = src[i] == 0 ? '0' : '1';
// 		// printf("%c", res[i]);
// 	}
// 	// printf("\n\n");

// 	return res;
// }

void transferToTable(MinHeapNode* root, unsigned char array[], unsigned top, unsigned char* table[]) {

	// recursively assign 0 to left children
	// unsigned int index = (unsigned) root->data;
	// printf("???%u\n", root->data);
	// printf("root at %p\n", root);
	// printf("ascii: %u\n", index);
	// printf("valid?: %d\n", root->valid);
	// printf("Now array is %s\n", array);
	// printf("top: %d\n", top);
	if (root->left) {
		array[top] = '0';
		transferToTable(root->left, array, top + 1, table);
	}

	// recursively assign 1 to right children

	if (root->right) {
		array[top] = '1';
		transferToTable(root->right, array, top + 1, table);
	}

	// If isLeaf, then it contains one of the input unsigned characters, 
	// printout this unsigned character and its code from array[]

	if (isLeaf(root)) {
		// printf("FOUND: %c\n", root->data);
		// int index = (int) root->data;
		unsigned index = (unsigned) root->data;
		// printf("Index: %u\n", index);

		// table[index] = arrayToString(array, top);
		// table[index] = (unsigned char *)malloc((top + 1) * sizeof(unsigned char));
		table[index] = (unsigned char *)malloc((strlen(array) + 1) * sizeof(unsigned char));
		memset(table[index], 0, sizeof(unsigned char));
		strncpy(table[index], array, top);
		table[index][top] = 0;
		// printf("table[%d] size: %ld\t, top is %d\t, array size %ld\n", 
		// 	index, strlen(table[index]), top, strlen(array));
		// printf("array: %s\n", array);
		// printf("table: %s\n", table[index]);
	}
	
}

unsigned char** huffmanCodes(MinHeapNode** tree, unsigned char data[], unsigned freq[], int size) {
	
	
	MinHeapNode* root = buildHuffmanTree(data, freq, size);
	// printf("root inside at %p\n", root);
	// printf("&root inside at %p\n", &root);
	// root = buildHuffmanTree(data, freq, size);
	// tree = &root;
	*tree = root;
	
	
	unsigned char array[MAX_TREE_HEIGHT] = {0};
	// unsigned char array_2[MAX_TREE_HEIGHT] = {0};
	// printf("array: %s\n", array);
	unsigned top = 0;
	// int i;

	// printHuffman(root, array, top);
	transferToTable(root, array, top, table);
	// printf("root inside at %p\n", root);

	return table;
	// getHuffman(root, array, top);
	// HuffmanTable* table = initTable(256);
	// transferToTable(table, root, array, top);
	// return table;
}
// printf("%p\n", globalMinHeapPtr->array[i]);
void destroyMinHeap() {
	int i;
	// printf("glbptr: %p\n", globalMinHeapPtr);
	// printf("&glbptr: %p\n", &globalMinHeapPtr);
	
	// printf("%d\t", globalMinHeapPtr->size);
	// printf("%d\n", globalMinHeapPtr->capacity);
	for (i = 0; i < globalMinHeapPtr->capacity; i++) {
		// printf("%p\n", globalMinHeapPtr->array[i]);
		if (globalMinHeapPtr->array[i] != NULL) {
			// free(globalMinHeapPtr->array[i]);
			printf("%p\n", globalMinHeapPtr->array[i]);
		}
	}
	free(globalMinHeapPtr);
}

// void outputHuffman(int array[], int n) {
	
// 	int i;
// 	for (i = 0; i < n; ++i) {
		
// 	}
// }

// void setCode(HuffmanTable* huffmanTable, unsigned char token, int code[]) {

// }
// HuffmanTable* initTable(MinHeapNode* root, int array[], int top) {

// 	int i;
// 	for (i = 0; i < 256; i++) {
// 		huffmanTable[i].token = (unsigned char) i;
// 		huffmanTable[i].code = getCode();
// 	}
// }

void printTable(unsigned char* table[]) {

	int i;
	for (i = 0; i < 256; i++) {
		if (table[i] == 0) continue;
		printf("%d: %c: %s\n", i, (unsigned char) i, table[i]);
	}
}

void destroyHuffmanTree(MinHeapNode* root) {

	// MinHeapNode* root = *tree;

	if (root == NULL) return;

	if (root->left != NULL) {
		destroyHuffmanTree(root->left);
	} 

	if (root->right != NULL) {
		destroyHuffmanTree(root->right);
	}

	free(root);
	// if (isLeaf(root)) {
	// 	free(root);
	// }
}


// test main function
// int main() {

// 	unsigned char array[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
// 	int freq[] = { 5, 9, 12, 13, 16, 45 };

// 	int size = sizeof(array) / sizeof(array[0]);
// 	int i;

// 	// HuffmanTable* table = huffmanCodes(array, freq, size);
// 	unsigned char** table = huffmanCodes(array, freq, size);

// 	printf("Checking table:");
// 	for (i = 0; i < 256; i++) {
// 		if (table[i] == 0) continue;
// 		printf("%c: %s\n", (unsigned char) i, table[i]);
// 	}

// 	return 0;
// } 


