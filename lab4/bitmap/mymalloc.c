#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "bitmap.h"

char _heap[MEMSIZE] = {0};

// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
long get_index(void *ptr) {
    if(ptr == NULL)
        return -1;
    else
        return (long) ((char *) ptr - &_heap[0]);
}

void print_memlist() {
    // Implement this to call print_map from bitmap.c
    print_map(_heap, MEMSIZE / 8);
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
    long index = search_map(_heap, MEMSIZE / 8, size);
    allocate_map(_heap, index, size);
    return &_heap[index];
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
    long index = get_index(ptr);
    free_map(_heap, index, )
}

