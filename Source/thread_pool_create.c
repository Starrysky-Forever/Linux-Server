#include "Thread_pool.h"

thread_pool_t *thread_pool_create(int max, int min, int quemax)
{
	thread_pool_t *pool = NULL;

	if((pool = (thread_pool_t *)malloc(sizeof(thread_pool_t))) == NULL)
		thread_pool_error("thread pool create > malloc pool", -1, 0);
	pool->thread_shutdown = TRUE;
	pool->thread_max = max;
	pool->thread_min = min;
	pool->thread_busy = 0;
	pool->thread_alive = 0;
	pool->thread_exitcode = 0;

	if((pool->customer_tids = (pthread_t *)malloc(max * sizeof(pthread_t))) == NULL)
	{
		thread_pool_error("thread_pool_create > malloc tids", -1, 0);
	}

	if((pool->busines_queue = (busines_t *)malloc(quemax * sizeof(busines_t))) == NULL)
	{
		thread_pool_error("thread_pool_create > malloc business queue", -1, 0);
	}

	pool->b_front = 0;
	pool->b_rear = 0;
	pool->b_max = quemax;
	pool->b_cur = 0;

	if(pthread_mutex_init(&pool->thread_lock, NULL)!=0 || pthread_cond_init(&pool->not_full, NULL)!=0 || pthread_cond_init(&pool->not_empty, NULL)!=0)
	{
		printf("thread_pool_create > init lock or cond error\n");
		exit(0);
	}

	int err;
	if((err = pthread_create(&pool->manager_tid, NULL, thread_pool_manager_job, (void *)pool)) > 0)
	{
		thread_pool_error("thread_pool_create > create manager error", -1, err);
	}

	for(int i=0; i<pool->thread_min; i++)
	{
		if((err = pthread_create(&pool->customer_tids[i], NULL, thread_pool_customer_job, (void *)pool)) > 0)
		{
			thread_pool_error("thread_pool_create > create customer error", -1, err);
		}
		++pool->thread_alive;
	}

	return pool;
}
