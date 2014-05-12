/*
 * queue.cpp
 *
 *  Created on: Sep 25, 2012
 *      Author: xiangqian wang
 */

#include "queue.h"

void init_queue(queue_t *q) {
	q->head = NULL;
	q->tail = NULL;
}

void enqueue(queue_t *q, queue_node_t *node) {
	node->next = NULL;

	if (NULL == q->head) {
		q->head = node;
		q->tail = node;
	} else {
		q->tail->next = node;
		q->tail = node;
	}
}

queue_node_t *dequeue(queue_t *q) {
	if (NULL == q->head)			// empty queue
		return NULL;

	queue_node_t *qn = q->head;
	if (q->head == q->tail) {		// only one node in this queue
		q->head = q->tail = NULL;
	} else {
		q->head = q->head->next;
	}

	return qn;
}

