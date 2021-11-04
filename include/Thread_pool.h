#include <SOCKET.h>

#define SERVER_IP "192.168.32.128"
#define SERVER_PORT 8000
#define BUFSIZE 1500
#define BACKLOG 128
#define TIMEOUT 1
#define IPSIZE 16
#define EPOLLSIZE 200000
#define TRUE 1
#define FALSE 0
#define THREAD_CODE 10

int epfd;
pthread_mutex_t accept_lock;

typedef struct
{
	void *(*BUSINES_ADDR)(void *);
	void *BUSINES_ARG;
}busines_t;

typedef struct
{
	int thread_shutdown;
	int thread_max;
	int thread_min;
	int thread_busy;
	int thread_alive;
	int thread_exitcode;
	pthread_t *customer_tids;
	pthread_t manager_tid;

	busines_t *busines_queue;
	int b_front;
	int b_rear;
	int b_max;
	int b_cur;

	pthread_mutex_t thread_lock;
	pthread_cond_t not_full;
	pthread_cond_t not_empty;
}thread_pool_t;

void thread_pool_error(const char *strerr, int exitcode, int err);
int thread_pool_net_init(void);
int thread_pool_epoll_init(int Sockfd);
thread_pool_t *thread_pool_create(int Max, int Min, int QueMax);
int thread_pool_destroy(thread_pool_t *);
int thread_pool_epoll_listen(int epfd, thread_pool_t *p, int sockfd);
int thread_pool_add_busines(thread_pool_t *, busines_t);
void *thread_pool_customer_job(void *arg);
void *thread_pool_manager_job(void *arg);
int if_thread_alive(pthread_t);
void *BUSINES_ACCEPT(void *arg);
void *BUSINES_RESPONSE(void *arg);
