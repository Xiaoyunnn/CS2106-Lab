#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "llist.h"

char _heap[MEMSIZE] = {0};
TNode *_memlist = NULL; // To maintain information about length

// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
long get_index(void *ptr) {
    if(ptr == NULL)
        return -1;
    else
        return (long) ((char *) ptr - &_heap[0]);
}

void print_node(TNode *node) {
    if(node != NULL) {
        printf("Status: %s Start index: %d Length: %ld\n", 
            node->pdata->status == 0 ? FREE : ALLOCATED, 
            node->key, 
            node->pdata->len);
    } else {
        printf("Unable to find node.\n");
    }
}

// Prints the memlist.
void print_memlist() {
    process_list(_memlist, print_node);
    // long start = _memlist->tail->pdata->start_index + _memlist->tail->pdata->len;
    long start = _memlist->tail->key + _memlist->tail->pdata->len;
    long len = MEMSIZE - start;
    printf("Status: %s Start index: %ld Length: %ld\n", 
            FREE, start, len);
};

// Searches for the start index for the required size of memory.
long search_heap(size_t size) {
    int count = 0;
    long start = 0;
    for (long i = 0; i < MEMSIZE; i++) {
        if (_heap[i] == 0) {
            count++;
            if (count == size) {
                return start;
            }
        } else {
            start = i + 1;
        }
    }
    return -1;
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
    long index = search_heap(size);
    // printf("index: %ld\n", index);
    if (index == -1) {
        return NULL;
    }
    for (long i = index; i < index + size; i++) {
        _heap[i] = 1;
    }

    TNode *free_node = find_node(_memlist, index);
    if (free_node != NULL) {
        // printf("**************** free node ************\n");
        if (free_node->pdata->len > size) {
            // free_node->pdata->start_index += size;
            free_node->key += size;
            free_node->pdata->len -= size;
        } else {
            // printf("delete node with size %ld\n", free_node->pdata->len);
            delete_node(&_memlist, free_node);
        }
    }
    TData *data;
    TNode *node;
    data = (TData *) malloc(sizeof(TData));
    data->len = size;
    // data->start_index = index;
    data->status = 1;
    node = make_node(index, data);
    insert_node(&_memlist, node, ASCENDING);
    return &_heap[index];
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
    long index = get_index(ptr);
    printf("index to free: %ld\n", index);
    if (index == -1) {
        return;
    }
    TNode *node = find_node(_memlist, index);
    
    int size = node->pdata->len;
    // size_t
    for (long i = index; i < index + size; i++) {
        _heap[i] = 0;
    }
    // TODO: tnode with index deleted after merging, but the deleted node still has pointer in heap
    node->pdata->status = 0;
    if (node->prev == NULL) {
        node->pdata->status = 0;
        node->pdata->len += node->next->pdata->len;
        merge_node(_memlist, node, SUCCEEDING);
    } else if (node->prev->pdata->status == 0) {
        node->prev->pdata->len += size;
        merge_node(_memlist, node, PRECEDING);
    }
}

