//============================================================================
// Name        : thread_pool.cpp
// Author      : xiangqian wang
// Version     :
// Copyright   : @2012
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include "thread_pool.h"

int init_thread_pool(thread_pool_t *pool, void *(*thread_function)(void *)) {
	if (pthread_mutex_init(&(pool->mutex), NULL))
		return EXIT_FAILURE;

	if (pthread_cond_init(&(pool->cond), NULL))
		return EXIT_FAILURE;

	init_queue(&pool->queue);

	thread_node_t *thread_node;
	for (int i = 0; i != NUM_OF_THREADS; ++i) {
		thread_node = (thread_node_t *) malloc(sizeof(thread_node_t));
		if (thread_node == NULL)
			return EXIT_FAILURE;

		// pthread_mutex_lock(&(pool->mutex));
		enqueue(&(pool->queue), (queue_node_t *) thread_node);			// cast my_thread to queue_node * to enqueue
		pool->thread_count++;
		// pthread_mutex_unlock(&(pool->mutex));

		thread_node->thread_index = i;
		if (pthread_create(&thread_node->thread_id, NULL, thread_function,
				(void *) thread_node))
			return EXIT_FAILURE;

		printf("thread %d is put into thread pool\n", thread_node->thread_index);
		fflush(stdout);
	}

	return EXIT_SUCCESS;
}

int destroy_thread_pool(thread_pool_t *pool) {
	if (pthread_mutex_destroy(&(pool->mutex)))
		return EXIT_FAILURE;

	if (pthread_cond_destroy(&(pool->cond)))
		return EXIT_FAILURE;

	thread_node_t *thread_node;
	while (pool->thread_count--) {
		// pthread_mutex_lock(&(pool->mutex));
		thread_node = (thread_node_t *) dequeue(&(pool->queue));
		// pthread_mutex_unlock(&(pool->mutex));

		pthread_join(thread_node->thread_id, NULL);
		printf("thread %d is joined\n", thread_node->thread_index);
		fflush(stdout);
		free(thread_node);
	}

	return EXIT_SUCCESS;
}

