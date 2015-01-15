#include <stdio.h>
#include <string.h>

char* get_host_url(char* url, char* url_host)
{
	char *url_tmp = url_host;
	if(strlen(url) == 0)
	{
		return "";
	}

	do
	{
		if(*url != '/')
		{
			*url_tmp++ = *url;
		}
		else
		{
			break;
		}
	} while (url++);

	return url_host;
}

int main(int argc, char** argv)
{
	char *url = "www.baidu.com/xfdsafds?xxxx&fdsa/ffds/fds";
	char url_host[50] = {'\0'};


	printf("%s\n", get_host_url(url, url_host));
	return 0;
}
