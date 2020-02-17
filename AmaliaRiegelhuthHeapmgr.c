#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void  initmemory(int size);  // initialize a simulated heap
void *myalloc(int length);   // allocate a block of length (or more) bytes
void  myfree(void *ptr);     // free an allocated block
void  coalesce();            // go through the heap, coalescing unallocated blocks
void  printallocation();  // print info about the simulated heap, for debugging purposes

// The following functions are needed for HW6.
// You should implement them now, as they will also be useful to help you implement HW5.

int  isAllocated(int *p);  // is the block at location p allocated?
int *nextBlock(int *p);    // return a pointer to the block that follows p
int *firstBlock();         // return a pointer to the first block on the heap
int *lastBlock();          // return a pointer to the sentinel block

int *firstBlk = NULL;
int *sentBlk = NULL;


void initmemory(int size) {
	int extra = 8 - ((size + 12) % 8);
	if (extra == 8) {
		extra = 0;
	} 
	int *heap = (int*)malloc(size + 12 + extra);
	firstBlk = heap + 1;
	*firstBlk = (size + extra + 4);
	sentBlk = nextBlock(firstBlk);
}

void *myalloc(int length) {
	int extra = 8 - ((length + 4) % 8);
	if (extra == 8) {
	extra = 0;
	}
	int size = length + 4 + extra;
	int *p1 = firstBlk;
	bool foundSpace = false;
	while((*p1 != 0) && !foundSpace) {
		if ((isAllocated(p1) == 0) && (((*p1)/8*8) >= size)) {
		*(p1 + size/4) = (*p1) - size;
		*p1 = size + 1;
		foundSpace = true;
		} else {
			p1 = nextBlock(p1);		
		}
	}
	if (!foundSpace) {
		return NULL;
	} else {
		return p1 + 1;
	}
}

void myfree(void *ptr) {
	int *p = (int*)ptr;
	*(p - 1) = *(p - 1) - 1;
	
}

void coalesce() {
	int *ptr = firstBlk;
	while(ptr != sentBlk) {
		int header = *ptr;
		if ((isAllocated(ptr) == 0) && (isAllocated(nextBlock(ptr)) == 0) && (nextBlock(ptr) != sentBlk)) {
			*ptr = header + *(ptr + header/4);
			
		} else {
			ptr = nextBlock(ptr);
		}
	}
}

void  printallocation() {
	int *ptr = firstBlk;
	int i = 0;
	while(ptr != sentBlk) {
		int header = *ptr;
		if(isAllocated(ptr) == 0) {
			printf("Block %d: size %-2d		unallocated\n", i, header);
		} else {
			printf("Block %d: size %-2d		allocated\n", i, (header - 1));
		}
		ptr = nextBlock(ptr);
		i++;
	}
}


int  isAllocated(int *p) {  
	if ((*(p) % 2) == 0) {
		return 	0;
	} else {
		return 1;
	}
}
int *nextBlock(int *p) {    
	return p + (*p)/4;
}
int *firstBlock() {         
	return firstBlk;
}
int *lastBlock() {          
	return sentBlk;
}




















