#include "Thread_pool.h"

int thread_pool_net_init(void)
{
	int sockfd;
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);  //设置端口
	inet_pton(AF_INET, SERVER_IP, &serveraddr.sin_addr.s_addr);

	sockfd = SOCKET(AF_INET, SOCK_STREAM, 0);

	BIND(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

	LISTEN(sockfd, BACKLOG);

	return sockfd;
}
