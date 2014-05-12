/*
 * work_pool.cpp
 *
 *  Created on: Sep 25, 2012
 *      Author: xiangqian wang
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "work_pool.h"
#include "queue.h"
#include "thread_pool.h"

#include <math.h>

int init_work_pool(work_pool_t *pool) {
	if (pthread_mutex_init(&(pool->mutex), NULL))
		return EXIT_FAILURE;

	if (pthread_cond_init(&(pool->cond), NULL))
		return EXIT_FAILURE;

	pool->activate = 0;

	init_queue(&pool->queue);

	work_node_t *work_node;
	// pthread_mutex_lock(&(pool->mutex));
	for (long i = 0; i != NUM_OF_WORKS; ++i) {
		work_node = (work_node_t *) malloc(sizeof(work_node_t));
		if (work_node == NULL) {
			printf("error malloc work node\n");
			fflush(stdout);
			break;
		}

		work_node->work_num = i;
		enqueue(&(pool->queue), (queue_node_t *) work_node);// cast my_work to queue_node* to enqueue
		pool->work_count++;
	}
	// pthread_mutex_unlock(&(pool->mutex));

	// broadcast signal to all waiting process after new work added
	// pthread_cond_broadcast(&(pool->cond));

	return EXIT_SUCCESS;
}

int destroy_work_pool(work_pool_t *pool) {
	if (pthread_mutex_destroy(&(pool->mutex)))
		return EXIT_FAILURE;

	if (pthread_cond_destroy(&(pool->cond)))
		return EXIT_FAILURE;

	pool->activate = 0;

	work_node_t *work_node;
	pthread_mutex_lock(&(pool->mutex));
	printf("work count is %ld\n", work_pool.work_count);
	fflush(stdout);
	while (pool->work_count--) {
		work_node = (work_node_t *) dequeue(&(pool->queue));
		free(work_node);
	}
	pthread_mutex_unlock(&(pool->mutex));

	return EXIT_SUCCESS;
}

int activate_work_pool(work_pool_t *pool) {
	if (pthread_mutex_lock(&(pool->mutex)))
		return EXIT_FAILURE;

	pool->activate = 1;

	pthread_mutex_unlock(&(pool->mutex));

	// broadcast signal to all waiting process after activate state changed
	pthread_cond_broadcast(&(pool->cond));

	return EXIT_SUCCESS;
}

int deactivate_work_pool(work_pool_t *pool) {
	if (pthread_mutex_lock(&(pool->mutex)))
		return EXIT_FAILURE;

	pool->activate = 0;

	pthread_mutex_unlock(&(pool->mutex));

	// broadcast signal to all waiting process after activate state changed
	pthread_cond_broadcast(&(pool->cond));

	return EXIT_SUCCESS;
}

long fibonacci(int n) {
	long n1 = 1, n2 = 1, n3 = 1;

	if ((1 == n) || (2 == n)) {
		return n;
	}

	for (int i = 3; i <= n; ++i) {
		n1 = n2 + n3;
		n3 = n2;
		n2 = n1;
	}

	return n1;
}

void c_mente_carlo(int iter) {
	if (iter <= 0) {
		return;
	}

	double x, y;
	int i, count = 0; /* # of points in the 1st quadrant of unit circle */
	double z;
	double pi;

	/* initialize random numbers */
	srand (35791246);
	count = 0;
	for (i = 0; i < iter; i++) {
		x = (double) rand() / RAND_MAX;
		y = (double) rand() / RAND_MAX;
		z = x * x + y * y;
		if (z <= 1)
			count++;
	}

	pi = (double) count / iter * 4;

	// printf("# of trials= %d , estimate of pi is %g \n", iter, pi);
	fflush(stdout);

	return;
}

// thread function, accept a thread node pointer parameter
void *thread_function(void *tn) {
	// thread_node_t *thread_node = (thread_node_t *) tn;
	work_node_t *work_node;

	// lock mutex before calling cond_wait;
	pthread_mutex_lock(&(work_pool.mutex));

	// process the work pool while its state is activate
	while (work_pool.activate) {
		// wait for signal to wake up. the signal comes if deactivate work pool or new work is put in work pool
		while (work_pool.activate && (work_pool.queue.head == NULL))
			pthread_cond_wait(&(work_pool.cond), &(work_pool.mutex));

		// end loop if the work pool is deactivated (by main thread)
		if (!work_pool.activate)
			break;

		// get one work item and process it
		work_node = (work_node_t *) dequeue(&(work_pool.queue));
		work_pool.work_count--;
		int num = work_node->work_num;
		work_pool.work_processed++;

		// printf("work %d is processed by thread %d\n", work_node->work_num,
		//		thread_node->thread_num);
		// fflush(stdout);

		// release memory allocated in main function
		free(work_node);

		// the mutex is locked after cond_wait, so unlock it here
		pthread_mutex_unlock(&(work_pool.mutex));

		// put some load
		// fibonacci(work_node->work_num);
		c_mente_carlo(1000000);

		pthread_cond_signal(&work_pool.cond);

		// lock the mutex before calling cond_wait
		pthread_mutex_lock(&(work_pool.mutex));
	}

	// assert all the work is done. otherwise we must free remain work memory (in the main thread) explicitly
	// assert(work_pool.queue.head == NULL);
	pthread_mutex_unlock(&(work_pool.mutex));

	return NULL;
}



