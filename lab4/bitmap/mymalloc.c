#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "bitmap.h"
#include "llist.h"

char _heap[MEMSIZE] = {0};
TNode *_memlist = NULL;

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
    // printf("index: %ld\n", index);
    if (index == -1) {
        return NULL;
    }
    allocate_map(_heap, index, size);
    TData *data;
    TNode *node;
    data = (TData *) malloc(sizeof(TData));
    data->len = size;
    node = make_node(index, data);
    insert_node(&_memlist, node, ASCENDING);
    // printf("size of tdata: %lu\n", sizeof(TData));
    // printf("size of tnode: %lu\n", sizeof(TNode));
    return &_heap[index];
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
    long index = get_index(ptr);
    if (ptr == NULL || index == -1) {
        return;
    }
    TNode *node = find_node(_memlist, index);
    if (node != NULL) {
        int size = node->pdata->len;
        free_map(_heap, index, size);
        delete_node(&_memlist, node);
    }
}

