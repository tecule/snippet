/*
 * thread_pool.h
 *
 *  Created on: Sep 25, 2012
 *      Author: xiangqian wang
 */

#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include "queue.h"
#include <pthread.h>

#define NUM_OF_THREADS	2

typedef struct _thread_node {
	queue_t *next_thread;
	int thread_index;
	pthread_t thread_id;
} thread_node_t;

typedef struct _thread_pool {
	queue_t queue;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int thread_count;
} thread_pool_t;

int init_thread_pool(thread_pool_t *, void *(*)(void *));
int destroy_thread_pool(thread_pool_t *);

#endif /* THREAD_POOL_H_ */
