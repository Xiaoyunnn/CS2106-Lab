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
    if (_memlist == NULL) {
        TData *ini_data = (TData *) malloc(sizeof(TData));
        TNode *node;
        ini_data->len = MEMSIZE;
        ini_data->status = 0;
        node = make_node(0, ini_data);
        insert_node(&_memlist, node, ASCENDING);
    }
    process_list(_memlist, print_node);
};

// Searches for the start index for the required size of memory.
long search_heap(size_t size) {
    TNode *trav = _memlist;

    int diff = MEMSIZE;
    int index = -1;
    while(trav != NULL) {
        if ((trav->pdata->status == 0) && (trav->pdata->len >= size)) {
            if ((trav->pdata->len - size) < diff) {
                diff = trav->pdata->len - size;
                index = trav->key;
            }
        }
        trav = trav->next;
    }

    return index;
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
    if (_memlist == NULL) {
        TData *ini_data = (TData *) malloc(sizeof(TData));
        TNode *node;
        ini_data->len = MEMSIZE;
        ini_data->status = 0;
        node = make_node(0, ini_data);
        insert_node(&_memlist, node, ASCENDING);
    }

    long index = search_heap(size);

    TNode *free_node = find_node(_memlist, index);
    if (free_node != NULL) {
        if (free_node->pdata->len > size) {
            free_node->key += size;
            free_node->pdata->len -= size;
        } else {
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
    if (index == -1) {
        return;
    }
    TNode *node = find_node(_memlist, index);
    
    if (node != NULL) {
        int size = node->pdata->len;
        // size_t
        for (long i = index; i < index + size; i++) {
            _heap[i] = 0;
        }
        node->pdata->status = 0;
        if (node->prev == NULL) {
            // head of list
            node->pdata->status = 0;
            while (node->next->pdata->status == 0) {
                node->pdata->len += node->next->pdata->len;
                merge_node(_memlist, node, SUCCEEDING);
            }
        } else {
            while ((node->prev != NULL) && (node->prev->pdata->status == 0)) {
                node->prev->pdata->len += size;
                merge_node(_memlist, node, PRECEDING);
                node = node->prev;
            }
            while ((node->next != NULL) && (node->next->pdata->status == 0)) {
                node->pdata->len += node->next->pdata->len;
                merge_node(_memlist, node, SUCCEEDING);
            }
        }
        
    }
}

