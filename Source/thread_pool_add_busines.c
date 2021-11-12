#include <Thread_pool.h>

int thread_pool_add_busines(thread_pool_t *pool, busines_t bus)
{
	if(pool->thread_shutdown)
	{
		pthread_mutex_lock(&pool->thread_lock);
		//判断工作条件	
		while(pool->b_cur == pool->b_max)
		{
			printf("服务器:[生产者] 不满足工作条件，挂起线程..\n");
			pthread_cond_wait(&pool->not_full, &pool->thread_lock);
		}

		pool->busines_queue[pool->b_front].BUSINES_ADDR = bus.BUSINES_ADDR;
		pool->busines_queue[pool->b_front].BUSINES_ARG = bus.BUSINES_ARG;
		++pool->b_cur;

		pool->b_front = (pool->b_front + 1) % pool->b_max; //环形偏移

		pthread_mutex_unlock(&pool->thread_lock);

		printf("服务器:生产者添加任务完成，唤醒一个消费者..\n");
		pthread_cond_signal(&pool->not_empty);
	}
	else
	{
		return -1;
	}

	return 0;
}
