/*
 * main.cpp
 *
 *  Created on: Sep 25, 2012
 *      Author: xiangqian wang
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include "work_pool.h"
#include "thread_pool.h"

#define dbort() { printf("abort at line %d in file %s\n", __LINE__, __FILE__); abort();}

thread_pool_t thread_pool;
work_pool_t work_pool;

int main(void) {
	printf("initialize work_pool ...\n");
	fflush(stdout);
	if (init_work_pool(&work_pool))
		dbort()

	printf("activate work_pool ...\n");
	fflush(stdout);
	activate_work_pool(&work_pool);

	printf("initialize thread_pool ...\n");
	fflush(stdout);
	if (init_thread_pool(&thread_pool, thread_function)) {
		printf("error initialize thread pool, start cleaning up ...\n");
		fflush(stdout);

		destroy_thread_pool(&thread_pool);
		destroy_work_pool(&work_pool);
		dbort()
	}

	/*
	printf("wait for processing ...\n");
	fflush(stdout);
	sleep(5);
	*/

	pthread_mutex_lock(&work_pool.mutex);
	while ((work_pool.work_processed < 100) && (work_pool.queue.head != NULL)) {
		// printf("work processed is %ld\n", work_pool.work_processed);
		// fflush(stdout);

		pthread_cond_wait(&work_pool.cond, &work_pool.mutex);
	}
	pthread_mutex_unlock(&work_pool.mutex);

	printf("deactivate work pool ...\n");
	fflush(stdout);
	deactivate_work_pool(&work_pool);

	printf("destroy thread pool ...\n");
	fflush(stdout);
	destroy_thread_pool(&thread_pool);

	printf("destroy work pool ...\n");
	fflush(stdout);
	destroy_work_pool(&work_pool);

	return EXIT_SUCCESS;
}
