#include <Thread_pool.h>

int thread_pool_epoll_listen(int epfd, thread_pool_t *p, int sockfd)
{
	//sockfd 实则为serverfd 用于判断就绪
	int readycode;
	struct epoll_event ready_array[EPOLLSIZE];
	int i;

	busines_t bs;

	//主控线程的监听与生产
	while(p->thread_shutdown)
	{
		if((readycode = epoll_wait(epfd, ready_array, EPOLLSIZE, -1)) > 0)
		{
			i = 0;
			while(readycode)
			{
				if(ready_array[i].data.fd == sockfd)
				{
					bs.BUSINES_ADDR = BUSINES_ACCEPT;
					bs.BUSINES_ARG = (void *)&(ready_array[i].data.fd);
					//添加业务A（链接）
					thread_pool_add_busines(p, bs);
				}
				else
				{
					bs.BUSINES_ADDR = BUSINES_RESPONSE;
					bs.BUSINES_ARG = (void *)&(ready_array[i].data.fd);
					//添加业务B（处理）
					thread_pool_add_busines(p, bs);
				}
				i++;
				readycode--;
			}
		}
		else if(readycode == -1)
		{
			thread_pool_error("thread_pool_epoll_listen > epoll_wait error", -1, 0);
		}
	}
	return 0;
}
