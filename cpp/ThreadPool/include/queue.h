/*
 * queue.h
 *
 *  Created on: Sep 25, 2012
 *      Author: xiangqian wang
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stddef.h>

typedef struct node {
	struct node *next;
} queue_node_t;

typedef struct queue {
	queue_node_t *head;
	queue_node_t *tail;
} queue_t;

void init_queue(queue_t *);
void enqueue(queue_t *, queue_node_t *);
queue_node_t *dequeue(queue_t *);

#endif /* QUEUE_H_ */
