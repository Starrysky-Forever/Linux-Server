#include "Thread_pool.h"

void thread_pool_error(const char *strerr, int exitcode, int err)
{
	if(errno == 0)
	{
		perror(strerr);
		exit(0);
	}
	else
	{
		printf("%s:%s\n", strerr, strerror(err));
		pthread_exit((void *)exitcode);
	}
}
