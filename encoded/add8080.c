#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "gb2312_unicode_table.h"

/*gb2312编码表+0x8080对应汉字*/

int main(void)
{
	int len = sizeof(code_table2) / sizeof(struct gb2312_unicode);
	int fd = open("./gb2312_unicode_table.txt", O_CREAT|O_RDWR|O_APPEND, 0666);
	char buf[20] = {'\0'};
	int i = 0;
	for(i = 0; i < len; i++)
	{
		sprintf(buf, "{0x%04x,\t0x%04x},", (code_table2[i].gb2312 + 0x8080), code_table2[i].unicode);
		write(fd, buf, strlen(buf));
		write(fd, "\n", 1);
	}
	close(fd);
	return 0;
}
