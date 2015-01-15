#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
int main(int argc, char** argv)
{
	char *ptr;
	char **pptr;
	struct hostent *hptr;
	char str[20] = {'\0'};
	if(argc != 2)
	{
		printf("eg: ./a.out www.baidu.com\n");
		exit(0);
	}
	ptr = *++argv;
	printf("You put DNS domain is: %s\n", ptr);
	if((hptr = gethostbyname(ptr)) == NULL)
	{
		printf("error!\n");
		exit(0);
	}
	printf("official hostname:%s\n", hptr->h_name);
	for(pptr = hptr->h_aliases; *pptr != NULL; pptr++)
		printf("\talias: %s\n", *pptr);
	switch(hptr->h_addrtype)
	{
		case AF_INET:
			pptr = hptr->h_addr_list;
			for(; *pptr != NULL; pptr++)
				printf("\taddress: %s\n", inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
			break;
		default:
			printf("unknown address type!\n");
			break;
	}
	return 0;
}
