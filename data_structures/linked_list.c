#include <stdlib.h>
#include <stdio.h>

#include "linked_list.h"

struct LinkedList linked_list_constructor() {
    struct LinkedList ll;
    ll.head = NULL;
    ll.length = 0;
    
    ll.insert = insert_ll;
    ll.remove = remove_node_ll;
    ll.retrieve = retrieve_ll;
    ll.sort = merge_sort_ll;
    ll.search = binary_search_ll;
    
    return ll;
}

void linked_list_destructor(struct LinkedList *linked_list) {
    for (int i = 0; i < linked_list->length; i++) {
        linked_list->remove(linked_list, 0);
    }
}

struct Node * create_node_ll(void *data, unsigned long size) {
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    *new_node = node_constructor(data, size);
    return new_node;
}

void destroy_node_ll(struct Node *node_to_destroy) {
    node_destructor(node_to_destroy);
}

struct Node * iterate_ll(struct LinkedList *linked_list, int index) {
    if (index < 0 || index >= linked_list->length) return NULL;

    struct Node *current = linked_list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current;
}

void insert_ll(struct LinkedList *linked_list, int index, void *data, unsigned long size) {
    struct Node *node_to_insert = create_node_ll(data, size);
    if (index == 0) {
        node_to_insert->next = linked_list->head;
        linked_list->head = node_to_insert;
    } else {
        struct Node *current = iterate_ll(linked_list, index - 1);
        node_to_insert->next = current->next;
        current->next = node_to_insert;
        
    }
    linked_list->length += 1;
}

void remove_node_ll(struct LinkedList *linked_list, int index) {
    if (index == 0) {
        struct Node *node_to_remove = linked_list->head;
        if (node_to_remove) {
            linked_list->head = node_to_remove->next;
            destroy_node_ll(node_to_remove);
        }
    } else {
        struct Node *current = iterate_ll(linked_list, index - 1);
        struct Node *node_to_remove = current->next;
        current->next = node_to_remove->next;
        destroy_node_ll(node_to_remove);
    }
    linked_list->length -= 1;
}

void * retrieve_ll(struct LinkedList *linked_list, int index) {
    struct Node *current = iterate_ll(linked_list, index);
    if (current) {
        return current->data;
    } else {
        return NULL;
    }
}

void merge_sort_ll(struct LinkedList *linked_list, int (*compare)(void *a, void *b)) {
    // O(n log n) time, O(n) space
    linked_list->head = merge_sort_recursive(linked_list->head, compare);
}

struct Node *merge_sort_recursive(struct Node *head, int (*compare)(void *a, void *b)) {
    if (!head || !head->next) return head;

    // split list into two halves
    struct Node *mid = merge_sort_get_mid(head);
    struct Node *second_half = mid->next;
    mid->next = NULL;

    // sort both halves
    struct Node *l = merge_sort_recursive(head, compare);
    struct Node *r = merge_sort_recursive(second_half, compare);

    return merge_sorted_lists(l, r, compare);
}

struct Node *merge_sort_get_mid(struct Node *head) {
    struct Node *slow = head;
    struct Node *fast = head;

    // love me some classic getting of mid
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

struct Node *merge_sorted_lists(struct Node *l, struct Node *r, int (*compare)(void *a, void *b)) {
    struct Node dummy;
    struct Node *tail = &dummy;
    dummy.next = NULL;

    // merge two lists
    while (l && r) {
        if (compare(l->data, r->data) <= 0) {
            tail->next = l;
            l = l->next;
        } else {
            tail->next = r;
            r = r->next; 
        }
        tail = tail->next;
    }

    // append remaining
    if (l) {
        tail->next = l;
    } else {
        tail->next = r;
    }

    return dummy.next;
}

short binary_search_ll(struct LinkedList *linked_list, void *query, int (*compare)(void *a, void *b)) {
    int mid = linked_list->length/2;
    int l = 0;
    int r = linked_list->length;
    while (r > l) {
        void *data = linked_list->retrieve(linked_list, mid);
        if (compare(data, query) == 1) {
            r = mid;
            if (mid != (l + mid) / 2) {
                mid = (l + mid) / 2;
            } else {
                break;
            }
        } else if (compare(data, query) == -1) {
            l  = mid;
            if (mid != (r + mid) / 2) {
                mid = (r + mid) / 2;
            } else {
                break;
            }
        } else {
            return 1;
        }
    }
    return 0;
}
