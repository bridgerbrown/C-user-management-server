#ifndef LinkedList_h
#define LinkedList_h

#include "../node.h"

struct LinkedList {
    struct Node *head;
    int length;
    
    void (*insert)(struct LinkedList *linked_list, int index, void *data, unsigned long size);
    void (*remove)(struct LinkedList *linked_list, int index);
    void * (*retrieve)(struct LinkedList *linked_list, int index);
    void (*sort)(struct LinkedList *linked_list, int (*compare)(void *a, void *b));
    short (*search)(struct LinkedList *linked_list, void *query, int (*compare)(void *a, void *b));
};

struct LinkedList linked_list_constructor(void);
void linked_list_destructor(struct LinkedList *linked_list);

struct Node * create_node_ll(void *data, unsigned long size);
void destroy_node_ll(struct Node *node_to_destroy);
struct Node * iterate_ll(struct LinkedList *linked_list, int index);
void insert_ll(struct LinkedList *linked_list, int index, void *data, unsigned long size);
void remove_node_ll(struct LinkedList *linked_list, int index);
void * retrieve_ll(struct LinkedList *linked_list, int index);

void merge_sort_ll(struct LinkedList *linked_list, int (*compare)(void *a, void *b));
struct Node *merge_sort_recursive(struct Node *head, int (*compare)(void *a, void *b));
struct Node *merge_sort_get_mid(struct Node *head);
struct Node *merge_sorted_lists(struct Node *l, struct Node *r, int (*compare)(void *a, void *b));

short binary_search_ll(struct LinkedList *linked_list, void *query, int (*compare)(void *a, void *b));
#endif 
