#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int fd[2];
	int r = socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
	if(r < 0)
	{
		perror("socketpair()");
		exit(1);
	}
	
	if (fork())
	{
		/*Parent process:echo client*/
		int val = 0;
		close(fd[0]);
		while(1) {
			sleep(1);
			++val;
			printf("Sending data:%d\n", val);
			write(fd[1], &val, sizeof(val));
			read(fd[1], &val, sizeof(val));
			printf("Data received:%d\n", val);
		}
	}
	else
	{
		/*Child process:echo server*/
		int val;
		close(fd[1]);
		while(1) {
			read(fd[0], &val, sizeof(val));
			++val;
			write(fd[0], &val, sizeof(val));
		}
	}
	return 0;
}
