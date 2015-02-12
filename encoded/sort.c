#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "gb2312_unicode_table.h"

/*
 * 将gb2312_unicode表按unicode排序,生成unicode_gb2312_table.h。
 * 手动将该表修改成数组格式.
 *
 * */

int main(void)
{
	struct gb2312_unicode temp;
	int num = sizeof(code_table2)/sizeof(struct gb2312_unicode);	
	int i = 0;
	int j = 0;
	for(i = 0; i < num; i++)
	{
		for(j = 1; j < num - i; j++)
		{
			if(code_table2[j - 1].unicode > code_table2[j].unicode)
			{
				temp.unicode = code_table2[j - 1].unicode;	
				temp.gb2312 = code_table2[j - 1].gb2312;
				code_table2[j - 1].unicode = code_table2[j].unicode;
				code_table2[j - 1].gb2312 = code_table2[j].gb2312;
				code_table2[j].unicode = temp.unicode;
				code_table2[j].gb2312 = temp.gb2312;
			}
		}
	}
#if 0
	for (i = 0; i < num; i++)
	{
		printf("{0x%04X,\t0x%04X},\t\n", code_table[i].unicode, code_table[i].gb2312);
	}
#endif

	int fd = open("./unicode_gb2312_table111.h", O_CREAT|O_RDWR|O_APPEND, 0666);
	char buf[100] = {'\0'};
	for(i = 0; i < num; i++)
	{
		memset(buf, 0, 100);
		sprintf(buf, "{0x%04x,\t0x%04x},", code_table2[i].unicode, code_table2[i].gb2312);
		write(fd, buf, strlen(buf));
		write(fd, "\n", 1);
	}

	close(fd);

	return 0;
}
