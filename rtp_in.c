#include <stdio.h>
#include <pthread.h>
#include "llist.h"

void *thr_rtp_in(void *arg)
{
	printf("rtp in start\n");

	while (true)
	{
	}
	return((void *)1);
}

bool rtp_in_mon(void)
{
	pthread_t	tid_rtp_in;
	int			err;
	
	err = pthread_create(&tid_rtp_in, NULL, thr_rtp_in, (void *)1);
	if (err != 0)
		perror("can't create thread rtp in: %s\n", strerror(err));

	return true;
}
