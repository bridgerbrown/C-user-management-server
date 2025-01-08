#ifndef QUEUE_H
#define QUEUE_H

#include "linked_list.h"

struct Queue {
		struct LinkedList list;
		
		void (*push)(struct Queue *queue, void *data, unsigned long size);
		void * (*peek)(struct Queue *queue);
		void (*pop)(struct Queue *queue);
};

struct Queue queue_constructor(void);
void queue_destructor(struct Queue *queue);
#endif
