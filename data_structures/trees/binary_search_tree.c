#include "binary_search_tree.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct BinarySearchTree binary_search_tree_constructor(int (*compare)(void *l, void *r)) {
	struct BinarySearchTree tree;
	tree.head = NULL;
	tree.compare = compare;
	tree.search = search_bst;
	tree.insert = insert_bst;
	return tree;
}

void bstree_destructor(struct BinarySearchTree tree) {
	recursive_tree_destruction(tree.head);
}

struct Node * create_node_bst(void *data, unsigned long size) {
	struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
	*new_node = node_constructor(data, size);
	return new_node;
}

void destroy_node_bst(struct Node *node_to_destroy) {
	node_destructor(node_to_destroy);
}

struct Node * iterate_bst(struct BinarySearchTree *tree, struct Node *current, void *data, int *direction) {
	// 1 = desired data is greater than node, -1 = less than, 0 = equal
	if (tree->compare(current->data, data) == 1) {
		if (current->next) {
			return iterate_bst(tree, current->next, data, direction);
		} else {
			*direction = 1;
			return current;
		}
	} else if (tree->compare(current->data, data) == -1) {
		if (current->previous) {
			return iterate_bst(tree, current->previous, data, direction);
		} else {
			*direction = -1;
			return current;
		}
	} else {
		*direction = 0;
		return current;
	}
}

void recursive_tree_destruction(struct Node *node) {
	if (node->previous) recursive_tree_destruction(node->previous);
	if (node->next) recursive_tree_destruction(node->next);
	destroy_node_bst(node);
}

void * search_bst(struct BinarySearchTree *tree, void *data) {
	int direction = 0;
	struct Node *current = iterate_bst(tree, tree->head, data, &direction);
	if (direction == 0) {
		return current->data;
	} else {
		return NULL;
	}
}

void insert_bst(struct BinarySearchTree *tree, void *data, unsigned long size) {
	if (!tree->head) {
		tree->head = create_node_bst(data, size);
	} else {
		int direction = 0;
		struct Node *current = iterate_bst(tree, tree->head, data, &direction);
		if (direction == 1) {
			current->next = create_node_bst(data, size);
		} else if (direction == -1) {
			current->previous = create_node_bst(data, size);
		}
	}
}

int bst_str_compare(void *l, void *r) {
	int comparison = strcmp(l, r);
	if (comparison > 0) {
		return 1;
	} else if (comparison < 0) {
		return -1;
	} else {
		return 0;
	}
}
