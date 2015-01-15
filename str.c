#include <stdio.h>

char* getstr(char* content, char* des)
{
	char *tmp = des;
	while(*content != '\0')
	{
		//if(*content != '\r' && *content != '\n')
		if(*content != 0x0d && *content != 0x0a)
		{
			*tmp++ = *content;
		}
		content++;
	}
	return des;
}
int main(int argc, char** argv)
{
	char* content = "\r\n\r\nfdsa\r\n";
	char des[20] = {'\0'};
	
	printf("abc%scdf\n", getstr(content, des));
	printf("abc%scdf\n", content);
	return 0;
}
