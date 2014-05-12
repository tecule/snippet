/*
 * work_pool.h
 *
 *  Created on: Sep 25, 2012
 *      Author: xiangqian wang
 */

#ifndef WORK_POOL_H_
#define WORK_POOL_H_

#include "queue.h"
#include <pthread.h>

#define NUM_OF_WORKS	6000000 // 6553600

typedef struct _work_node {
	queue_t *next_work;
	int work_num;
} work_node_t;

typedef struct _work_pool {
	queue_t queue;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	long work_count;
	long work_processed;
	int activate;
} work_pool_t;

extern work_pool_t work_pool;

int init_work_pool(work_pool_t *);
int destroy_work_pool(work_pool_t *);
int activate_work_pool(work_pool_t *);
int deactivate_work_pool(work_pool_t *);

long fibonacci(int);
void c_mente_carlo(int);
void *thread_function(void *);

#endif /* WORK_POOL_H_ */
