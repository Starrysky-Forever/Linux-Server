#include <Thread_pool.h>

void *BUSINES_ACCEPT(void *arg)
{
	struct sockaddr_in clientaddr;
	int clientfd;
	struct epoll_event tmp_env;   //添加节点用

	int serverfd = *(int *)arg;
	socklen_t addrsize = sizeof(clientaddr);
	pthread_mutex_lock(&accept_lock);
	clientfd = ACCEPT(serverfd, (struct sockaddr *)&clientaddr, &addrsize);
	pthread_mutex_unlock(&accept_lock);

	tmp_env.data.fd = clientfd;
	tmp_env.events = EPOLLIN|EPOLLET;  //边缘触发模式

	if((epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &tmp_env)) == -1)
	{
		thread_pool_error("BUSINES_ACCEPT > epoll_ctl error", -1, 0);
	}

	return NULL;
}
