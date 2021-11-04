#include <Thread_pool.h>

int thread_pool_epoll_init(int sockfd)
{
	struct epoll_event tmp_env;
	if((epfd = epoll_create(EPOLLSIZE)) == -1)
	{
		thread_pool_error("thread_pool_epoll_init > epoll create error", -1,0);
	}
	
	tmp_env.data.fd = sockfd;
	tmp_env.events = EPOLLIN|EPOLLET;

	if((epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &tmp_env)) == -1)
	{
		thread_pool_error("thread_pool_epoll_init >> epoll ctl error", -1, 0);
	}

	return 0;
}
