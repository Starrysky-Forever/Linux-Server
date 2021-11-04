#include "Thread_pool.h"

int if_thread_alive(pthread_t tid)
{
	pthread_kill(tid, 0);
	if(errno == ESRCH)
		return FALSE;
	else
		return TRUE;
}
