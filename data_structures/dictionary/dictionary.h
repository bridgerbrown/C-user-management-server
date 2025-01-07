#ifndef Dictionary_h
#define Dictionary_h

#include "../trees/binary_search_tree.h"
#include "../lists/linked_list.h"

struct Dictionary {
		struct BinarySearchTree binary_search_tree;
		struct LinkedList keys;
		void (*insert)(struct Dictionary *dictionary, void *key, unsigned long key_size, void *value, unsigned long value_size);
		void * (*search)(struct Dictionary *dictionary, void *key, unsigned long key_size);
};

struct Entry {
		void *key;
		void *value;
};

struct Entry entry_constructor(void *key, unsigned long key_size, void *value, unsigned long value_size);
void entry_destructor(struct Entry *entry);

struct Dictionary dictionary_constructor(int (*compare)(void *entry_one, void *entry_two));
void dictionary_destructor(struct Dictionary *dictionary);
void recursive_dictionary_destroy(struct Node *current);
void insert_dictionary(struct Dictionary *dictionary, void *key, unsigned long key_size, void *value, unsigned long value_size);
void *search_dictionary(struct Dictionary *dictionary, void *key, unsigned long key_size);
int compare_key_strings(void *entry_one, void *entry_two);
#endif
