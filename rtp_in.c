#include <stdio.h>
#include <pthread.h>
#include "llist.h"
#include "header.h"

extern llist *list_i;
extern llist *list_other
extern llist *list_net;
bool is_I(uint8_t *rtp_buf)
{
    nalu_header_t *nalu_header;
    fu_header_t   *fu_header;

    nalu_header = (nalu_header_t *)&rtp_buf[12];
    if (nalu_header->type == 28) 
    { 
        fu_header = (fu_header_t *)&rtp_buf[13];
        if ((fu_header->type & 0x1f) == 0x05)
        	return true;
    } else { 
    	if ((nalu_header->type & 0x1f) == 0x05)
    		return true;
    }

    return false;
}
void *thr_rtp_in(void *arg)
{
	unsigned char *ptr = NULL;
	int len;
	printf("rtp in start\n");

	while (true)
	{
		if ((len = socket_in(sock_rtp_in, (void **)&ptr, 2000, 1122)) > 0)
		{
			if (is_I(ptr))
				printf("this rtp is I , len %d\n", len);

			free(ptr);
		}
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
