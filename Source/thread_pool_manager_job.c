#include <Thread_pool.h>
		
void *thread_pool_manager_job(void *arg)
{
	pthread_detach(pthread_self());
	//管理者线程需要频繁使用阀值，进行扩容缩减判断(alive, busy, cur)
	//降低管理者对互斥锁的占用
	
	thread_pool_t *pool = (thread_pool_t *)arg;

	int alive, busy, cur;
	int flag, add;
	int err;
	while(pool->thread_shutdown)
	{
		pthread_mutex_lock(&pool->thread_lock);
		alive = pool->thread_alive;
		busy = pool->thread_busy;
		cur = pool->b_cur;
		pthread_mutex_unlock(&pool->thread_lock);

		if((cur >= alive-busy || (double)busy/alive*(double)100 >= 70) && alive + THREAD_CODE <= pool->thread_max)
		{
			for(flag=0, add=0; flag < pool->thread_max && add < THREAD_CODE; flag++)
			{
				if(pool->customer_tids[flag] == 0 || !if_thread_alive(pool->customer_tids[flag]))
				{
					if((err = pthread_create(&pool->customer_tids[flag], NULL, thread_pool_customer_job, (void *)pool)) > 0)
					{
						thread_pool_error("thread_pool_manager_job >> pthread_create error",-1,err);
					}
					add++;

					pthread_mutex_lock(&pool->thread_lock);
					++pool->thread_alive;
					pthread_mutex_unlock(&pool->thread_lock);
				}
			}
		}

		if(busy*2 <= alive-busy && alive-THREAD_CODE >= pool->thread_min)
		{
			pthread_mutex_lock(&pool->thread_lock);
			pool->thread_exitcode = THREAD_CODE;
			pthread_mutex_unlock(&pool->thread_lock);

			for(int i=0; i<THREAD_CODE; i++)
			{
				pthread_cond_signal(&pool->not_empty);
			}
		}
		sleep(TIMEOUT);
	}
	pthread_exit(NULL);
}
