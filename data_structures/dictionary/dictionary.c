#include "dictionary.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Dictionary Entry
struct Entry entry_constructor(void *key, unsigned long key_size, void *value, unsigned long value_size) {
	struct Entry entry;
	entry.key = malloc(key_size);
	entry.value = malloc(value_size);
	memcpy(entry.key, key, key_size);
	memcpy(entry.value, value, value_size);
	return entry;
}

void entry_destructor(struct Entry *entry) {
	free(entry->key);
	free(entry->value);
	free(entry);
}

// Dictionary
struct Dictionary dictionary_constructor(int (*compare)(void *key_one, void *key_two)) {
	struct Dictionary dictionary;
	dictionary.binary_search_tree = binary_search_tree_constructor(compare); // fast key lookups
	dictionary.keys = linked_list_constructor(); // maintaining insertion order
	dictionary.insert = insert_dictionary;
	dictionary.search = search_dictionary;
	return dictionary;
}

void dictionary_destructor(struct Dictionary *dictionary) {
	linked_list_destructor(&dictionary->keys);
	recursive_dictionary_destroy(dictionary->binary_search_tree.head);
}

void recursive_dictionary_destroy(struct Node *current) {
	if (current->previous) {
		recursive_dictionary_destroy(current->previous);
	}
	if (current->next) {
		recursive_dictionary_destroy(current->next);
	}
	entry_destructor((struct Entry *)current->data);
	free(current->data);
	free(current);
}

void * search_dictionary(struct Dictionary *dictionary, void *key, unsigned long key_size) {
	int value = 0;
	struct Entry searchable = entry_constructor(key, key_size, &value, sizeof(value));
	void * result = dictionary->binary_search_tree.search(&dictionary->binary_search_tree, &searchable);
	if (result) {
		return ((struct Entry *)result)->value;
	} else {
		return NULL;
	}
}

void insert_dictionary(struct Dictionary *dictionary, void *key, unsigned long key_size, void *value, unsigned long value_size) {
	struct Entry entry = entry_constructor(key, key_size, value, value_size);
	dictionary->binary_search_tree.insert(&dictionary->binary_search_tree, &entry, sizeof(entry));
	dictionary->keys.insert(&dictionary->keys, dictionary->keys.length, key, key_size);
}

int compare_key_strings(void *entry_one, void *entry_two) {
	if (strcmp((char *)(((struct Entry *)entry_one)->key), (char *)(((struct Entry *)entry_two)->key)) > 0) {
		return 1;
	} else if (strcmp((char *)(((struct Entry *)entry_one)->key), (char *)(((struct Entry *)entry_two)->key)) < 0) {
		return -1;
	} else {
		return 0;
	}
}
