#include <Thread_pool.h>


//消费者任务
void *thread_pool_customer_job(void *arg)
{
	//提高消费者线程的重用性，它可以获取多次业务并执行
	//消费者的任务参数为线程池地址，从线程池中获取任务执行
	//注意环形队列的偏移处理以及阀值设置，获取任务成功唤醒一个生产者
	pthread_detach(pthread_self());

	busines_t bs;
	thread_pool_t *pool = (thread_pool_t *)arg;
	while(pool->thread_shutdown)
	{
		pthread_mutex_lock(&pool->thread_lock);
		while(pool->b_cur == 0)
		{
			pthread_cond_wait(&pool->not_empty, &pool->thread_lock);
			if(pool->thread_exitcode > 0)
				break;
		}
		if(pool->thread_shutdown == 0)
		{
			--pool->thread_alive;
			pthread_mutex_unlock(&pool->thread_lock);
			pthread_exit(NULL);
		}
		if(pool->thread_exitcode > 0)
		{
			--pool->thread_alive;
			--pool->thread_exitcode;
			pthread_mutex_unlock(&pool->thread_lock);
			pthread_exit(NULL);
		}

		//工作条件满足.获取变量
		bs.BUSINES_ADDR = pool->busines_queue[pool->b_rear].BUSINES_ADDR;
		bs.BUSINES_ARG = pool->busines_queue[pool->b_rear].BUSINES_ARG;
		
		--pool->b_cur;
		
		//环形队列偏移
		pool->b_rear = (pool->b_rear + 1) % pool->b_max;

		++(pool->thread_busy);

		pthread_mutex_unlock(&pool->thread_lock);
		
		pthread_cond_signal(&pool->not_full);
		
		//执行任务
		bs.BUSINES_ADDR(bs.BUSINES_ARG);

		pthread_mutex_lock(&pool->thread_lock);
		--(pool->thread_busy);
		pthread_mutex_unlock(&pool->thread_lock);
	}

	pthread_exit(NULL);
}
