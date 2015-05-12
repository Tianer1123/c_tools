#include <stdio.h>
#include <stdlib.h>  
#include <unistd.h>  
#include <errno.h>  
#include <string.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <sys/epoll.h>  
#include <fcntl.h>  

#define MYPORT 8886
#define BACKLOG 5
#define BUF_SIZE 10000
#define MAXLINE 100
#define MAXCLIENT 5

int fd_access[BACKLOG];
int conn_amount;
int recv_number = 0;

void setnonblocking(int sock)
{
	int opts;

	opts = fcntl(sock, F_GETFL);
	if(opts < 0)
	{
		perror("fcntl(F_GETFL)");
		exit(1);
	}

	opts = opts | O_NONBLOCK;

	if(fcntl(sock, F_SETFL, opts) < 0)
	{
		perror("fcntl(F_SETFL)");
		exit(1);
	}

	
}

int main(void)
{
	int sock_fd, new_fd, listen_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	socklen_t sin_size;
	char buf[BUF_SIZE];
	int result;
	int i;
	int epfd, n;

	if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	setnonblocking(listen_fd);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(MYPORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))
			== -1)
	{
		perror("bind");
		exit(1);
	}


	if(listen(listen_fd, BACKLOG) == -1)
	{
		perror("listen");
		exit(1);
	}

	struct epoll_event ev, events[20];

	epfd = epoll_create(1024);

	ev.data.fd = listen_fd;
	ev.events = EPOLLIN|EPOLLET;

	epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev);

	sin_size = sizeof(client_addr);

	while(1)
	{
		result = epoll_wait(epfd, events, 20, -1);

		for (i = 0; i < result; ++i)
		{
			if(events[i].data.fd == listen_fd)
			{
				new_fd = accept(listen_fd, (struct sockaddr*)&client_addr,
						&sin_size);

				while(new_fd > 0) 
				{
					ev.data.fd = new_fd;
					ev.events = EPOLLIN|EPOLLET;

					epoll_ctl(epfd, EPOLL_CTL_ADD, new_fd, &ev);


					new_fd = accept(listen_fd, (struct sockaddr*)
							&client_addr, &sin_size);
				}
			}
			else if(events[i].events & EPOLLIN)
			{
				if((sock_fd = events[i].data.fd) < 0)
				{
					printf("fd < 0\n");

					continue;
				}

				n = recv(sock_fd, buf, sizeof(buf), 0);

				recv_number += n;

				sleep(2);

				if (n < 0)
				{
					printf("mark1\n");

					if(errno == ECONNRESET)
					{
						close(sock_fd);
						events[i].data.fd = -1;
						continue;
					}
					else
					{
						printf("sorry \n");
						continue;
					}
				}
				else if(n == 0)
				{
					close(sock_fd);
					events[i].data.fd = -1;
					continue;
				}
				
				buf[n] = '\0';

				printf("fd[%d]: %s\n", sock_fd, buf);

				ev.data.fd = sock_fd;
				ev.events = EPOLLOUT|EPOLLET;

				epoll_ctl(epfd, EPOLL_CTL_MOD, sock_fd, &ev);
			}
			else if (events[i].events & EPOLLOUT) 
			{
				sock_fd = events[i].data.fd;

				write(sock_fd, buf, n);
				
				ev.data.fd = sock_fd;
				ev.events = EPOLLIN|EPOLLET;

				epoll_ctl(epfd, EPOLL_CTL_MOD, sock_fd, &ev);
			}
		}
	}
	printf("errno:%d\n", errno);

	return 0;
}
