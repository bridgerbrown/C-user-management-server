#ifndef BinarySearchTree_h
#define BinarySearchTree_h

#include "../node.c"

struct BinarySearchTree {
		struct Node *head;	

		int (*compare)(void *l, void *r);
		void * (*search)(struct BinarySearchTree *tree, void *data);
		void (*insert)(struct BinarySearchTree *tree, void *data, unsigned long size);
};

struct BinarySearchTree binary_search_tree_constructor(int (*compare)(void *l, void *r));
void binary_search_tree_destructor(struct BinarySearchTree tree);

struct Node * create_node_bst(void *data, unsigned long size);
void destroy_node_bst(struct Node *node_to_destroy);

struct Node * iterate_bst(struct BinarySearchTree *tree, struct Node *current, void *data, int *direction);
void recursive_tree_destruction(struct Node *node);

void * search_bst(struct BinarySearchTree *tree, void *data);
void insert_bst(struct BinarySearchTree *tree, void *data, unsigned long size);
int bst_str_compare(void *l, void *r);
#endif
