#include <Thread_pool.h>

int thread_pool_add_busines(thread_pool_t *pool, busines_t bus)
{
	if(pool->thread_shutdown)
	{
		pthread_mutex_lock(&pool->thread_lock);
	
		while(pool->b_cur == pool->b_max)
		{
			pthread_cond_wait(&pool->not_full, &pool->thread_lock);
		}

		pool->busines_queue[pool->b_front].BUSINES_ADDR = bus.BUSINES_ADDR;
		pool->busines_queue[pool->b_front].BUSINES_ARG = bus.BUSINES_ARG;
		++pool->b_cur;

		pool->b_front = (pool->b_front + 1) % pool->b_max; //环形偏移

		pthread_mutex_unlock(&pool->thread_lock);
		pthread_cond_signal(&pool->not_empty);
	}
	else
	{
		return -1;
	}

	return 0;
}
