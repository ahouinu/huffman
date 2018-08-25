huffman: huffman.o minheap.o bitops.o kmp.o
	gcc -o huffman huffman.o minheap.o bitops.o kmp.o

huffman.o: huffman.c minheap.h
	gcc -c huffman.c

minheap.o: minheap.c minheap.h
	gcc -c minheap.c

bitops.o: bitops.c bitops.h
	gcc -c bitops.c

kmp.o: kmp.c kmp.h
	gcc -c kmp.c

clean:
	rm -f *.o