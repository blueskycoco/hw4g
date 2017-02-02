#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/socket.h>  
#include <arpa/inet.h>
#include <netdb.h>  
#include <string.h>  
#include <errno.h>
#include "llist.h"

llist *list_i;
llist *list_other
llist *list_net;
int sock_rtp_in = 0;
int sock_rtp_out = 0;
int sock_server_in = 0;
#define RTP_IN_PORT 1234
#define RTP_OUT_PORT 1235
#define SERVER_IN_PORT 1236

bool socket_out(int sock, void *data, int len)
{
	struct timeval tv;
	fd_set writefs;
	bool result = true;
	struct sockaddr_in sockaddr;

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(RTP_OUT_PORT);  
	sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  

	FD_ZERO(&writefs);
	FD_SET(sock, &writefs);

	tv.tv_sec = 10;
	tv.tv_usec = 0;
	if (select(sock + 1, NULL, &writefs,
				NULL, &tv) > 0)
	{
		if (FD_ISSET(sock, &writefs))
		{
			int status = sendto(sock, data, len, 0, 
				(struct sockaddr *)&sockaddr, 
				sizeof(struct sockaddr_in));
			if (status < 0)
			{
				result = false;
				perror("sendto error \n");
			}
		}
		else
			perror("some error happen in socket out\n");
	}
	else
		printf("timeout in socket out\n");
	
	return result;
}
int socket_in(int sock, void **data, int len, int port)
{
	char buf[2048] = {0};
	struct timeval tv;
	fd_set readfs;
	int read_len = 0;
	socklen_t clientlen = sizeof(struct sockaddr_in);;
	struct sockaddr_in sockaddr;

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);  
	sockaddr.sin_addr.s_addr = INADDR_ANY;  

	FD_ZERO(&readfs);
	FD_SET(sock, &readfs);

	tv.tv_sec = 10;
	tv.tv_usec = 0;
	if (select(sock + 1, &readfs, NULL,
				NULL, &tv) > 0)
	{
		if (FD_ISSET(sock, &readfs))
		{
			read_len = recvfrom(sock, buf, len, 0, 
				(struct sockaddr *)&sockaddr, &clientlen);
			if (read_len < 0)
			{
				perror("recvfrom error \n");
			}
			else
			{
				*data = malloc(read_len * sizeof(char));
				memcpy(*data, buf, read_len);
			}
		}
		else
			perror("some error happen in socket in\n");
	}
	else
		printf("read socket in timeout\n");
	return read_len;
}
int main(int argc, void *argv[])
{
	unsigned char buf[10] = {0};
	unsigned char *ptr = NULL;
	if((sock_rtp_in = socket(AF_INET,SOCK_DGRAM,0))==-1)
	{
		perror("create sock rtp in failed\n");
		return -1;
	}
	else
	{
		struct sockaddr_in sockaddr;
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_addr.s_addr = INADDR_ANY;
		sockaddr.sin_port = htons(1122);
		if(bind(sock_rtp_in, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr)) == -1)
		{
			perror("bind to port 1122 failed\n");
		}
	}

	if((sock_rtp_out = socket(AF_INET,SOCK_DGRAM,0))==-1)
	{
		perror("create sock rtp out failed\n");
		return -1;
	}

	if((sock_server_in = socket(AF_INET,SOCK_DGRAM,0))==-1)
	{
		perror("create sock server in failed\n");
		return -1;
	}
	else
	{
		struct sockaddr_in sockaddr;
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_addr.s_addr = INADDR_ANY;
		sockaddr.sin_port = htons(1123);
		if(bind(sock_server_in, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr)) == -1)
		{
			perror("bind to port 1123 failed\n");
		}
	}

	for (int i = 0; i < 10; i++)
		buf[i] = i;
	socket_out(sock_rtp_out, buf, 10);
	int len;
	if ((len = socket_in(sock_rtp_in, (void **)&ptr, 100, 1122)) > 0)
	{
		for (int i = 0; i < len; i++)
			printf("%c", ptr[i]);
		printf("\n");
		free(ptr);
	}
	close(sock_rtp_in);
	close(sock_rtp_out);
	close(sock_server_in);

    list_i 		= create_null_list_link();
    list_other 	= create_null_list_link();
    list_net 	= create_null_list_link();
	return 0;
}
