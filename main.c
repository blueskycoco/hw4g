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
	sockaddr.sin_addr.s_addr = inet_addr("192.168.1.123");  

	FD_ZERO(&writefs);
	FD_SET(sock, &writefs);

	tv.tv_sec = 10;
	tv.tv_usec = 0;
	if (select(sock, NULL, &writefs,
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
			printf("timeout in rtp out\n");
	}
	else
		perror("some error happen in rtp out\n");
	
	return result;
}
int socket_in(int sock, void *data, int len, int port)
{
	char buf[2048] = {0};
	struct timeval tv;
	fd_set readfs;
	int read_len = 0;
	socklen_t clientlen;
	struct sockaddr_in sockaddr;

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);  
	sockaddr.sin_addr.s_addr = INADDR_ANY;  

	FD_ZERO(&readfs);
	FD_SET(sock_rtp_in, &readfs);

	tv.tv_sec = 10;
	tv.tv_usec = 0;
	if (select(sock_rtp_in, &readfs, NULL,
				NULL, &tv) > 0)
	{
		if (FD_ISSET(sock_rtp_in, &readfs))
		{
			read_len = recvfrom(sock, buf, len, 0, 
				(struct sockaddr *)&sockaddr, &clientlen);
			if (read_len < 0)
			{
				perror("recvfrom error \n");
			}
			else
			{
				data = malloc(read_len * sizeof(char));
				memcpy(data, buf, read_len);
			}
		}
		else
			printf("read rtp in timeout\n");
	}
	else
		perror("some error happen in rtp in\n");
	return read_len;
}
int main(int argc, void *argv[])
{
	if((sock_rtp_in = socket(AF_INET,SOCK_DGRAM,0))==-1)
	{
		perror("create sock rtp in failed\n");
		return -1;
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
	return 0;
}
