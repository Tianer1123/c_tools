#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include <unistd.h>
#include<errno.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<netinet/in.h>  
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>

#define MAXLINE 4096  

int main(int argc, char** argv)  
{      
	int    sockfd;      
	char   sendline[4096];      
	char   recvline[4096];      
	struct sockaddr_in    servaddr;      

	if( argc != 2)  
	{      
		printf("usage: ./client <ipaddress>\n");      
		exit(0);      
	}      

	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)  
	{     
		printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);      
		exit(0);      
	}      

	memset(&servaddr, 0, sizeof(servaddr));      

	servaddr.sin_family = AF_INET;      
	servaddr.sin_port = htons(8886);      

	if( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)  
	{      
		printf("inet_pton error for %s\n",argv[1]);      
		exit(0);      
	}      
	if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)  
	{      
		printf("connect error: %s(errno: %d)\n",strerror(errno),errno);      
		exit(0);      
	}      
	//	printf("send msg to server: \n");     
	//	fgets(sendline, 4096, stdin);      

#if 0

	struct timeval timeout = {3,0}; 	
	socklen_t timeout_len = sizeof(timeout);

	setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, timeout_len);
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, timeout_len);


#endif
	strcpy(sendline, "HelloWorld\n");
	while(1) 
	{
		if( send(sockfd, sendline, strlen(sendline), 0) < 0)      
		{      
			printf("send msg error: %s(errno: %d)\n", 
					strerror(errno), errno);      
			exit(0);      
		}     
		printf("sendline:%s\n", sendline);
		recv(sockfd, recvline, sizeof(sendline), 0);

		printf("recvline:%s\n", recvline);

	}

	close(sockfd);      
	exit(0);  
}  
