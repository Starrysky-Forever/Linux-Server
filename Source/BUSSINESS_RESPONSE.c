#include <Thread_pool.h>

void *BUSINES_RESPONSE(void *arg)
{
	int clientfd = *(int *)arg;
	int rsize;
	char buffer[BUFSIZE];
	bzero(buffer, BUFSIZE);
	int flag;
	while((rsize = recv(clientfd, buffer, BUFSIZE, MSG_DONTWAIT)) > 0)
	{
		flag = 0;

		while(rsize > flag)
		{
			buffer[flag] = toupper(buffer[flag]);
			flag++;
		}

		send(clientfd, buffer, rsize, MSG_NOSIGNAL);
	}

	if(rsize == 0)
	{
		//客户端异常退出，将客户端从监听中删除
		if((epoll_ctl(epfd, EPOLL_CTL_DEL, clientfd, NULL)) == -1)
		{
			thread_pool_error("BUSINESS_RESPONSE > epoll_ctl error", -1, 0);
		}
		close(clientfd);
	}
	else if(rsize == -1)
	{
		if(errno == EAGAIN)
			printf("BUSINESS_RESPONSE > recv NONBLOCK return..\n");
		else
			thread_pool_error("BUSINESS_RESPONSE > recv error", -1, 0);
	}

	return NULL;
}
