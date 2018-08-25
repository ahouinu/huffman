// README.txt

======= Intro =======

Author: Tianpeng Chen z5176343


======= Usage =======

encode: ./huffman -e [input-src-path] [output-src-path]

decode: ./huffman -d [input-src-path] [output-src-path]

search: ./huffman -s [query-string] [file-path]


===== Structure =====

1. Program

huffman 	
	|--huffman.c 		//main
	|--minheap.h 		//minheap ADT
	|--minheap.c
	|--bitops.h 		//bit operation utility
	|--bitops.c
	|--kmp.h 			//kmp search
	|--kmp.c

2. Encoded Header

Header stores the whole frequency table for all ASCII characters as a 32-bit int, which is 256 * 4 = 1024 bytes in total.

After written header, encoder will write compressed data into output file from the 1025th byte.

So the encoded file is at least 1024 bytes.


===== Reflection =====

1. Be extremly careful with heap memory operations (malloc, memset, memcpy, etc...)

Fortunately you'll get a segmentation fault, which is not so hard to debug.
Sometimes your char array may be overwritten and it's much harder to find the problem...

2. Remember to initialze the heap space after a malloc operation. And always remember to free these spaces after used.

3. When dealing with binary files, be careful with strlen(), which interrupts the binary 0 to the terminating char '\0'.

4. In terms of design, the most tricky part is how to encode/decode/search without loading the whole file into memory (I did those byte-wise). 

As for implementation, the most tricky part is all the memory operations. C gives developer huge freedom to access the memory, but also the responsibility to access them properly.


===== Reference =====
1. Huffman Coding
https://www.geeksforgeeks.org/greedy-algorithms-set-3-huffman-coding/
2. KMP
https://www.geeksforgeeks.org/searching-for-patterns-set-2-kmp-algorithm/
3. fseek()
https://www.geeksforgeeks.org/fseek-in-c-with-example/
4. C Memory Layout
https://www.geeksforgeeks.org/memory-layout-of-c-program/
5. Pointers and Memory Leaks in C
https://www.ibm.com/developerworks/aix/library/au-toughgame/index.html
6. valgrind and massif tutorial by tutor Haojie
https://www.youtube.com/watch?v=kjkrpgWr9G4