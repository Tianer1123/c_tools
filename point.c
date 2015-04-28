#include <stdio.h>
#include <string.h>

char *test(char *src)
{
	char *a = NULL;
	a = src;
	return a;

}


int main(int argc, char** argv)
{
	char* p = NULL;
	char a[] = "hahaha";

	p = "";
	if(strlen(p) == 0 || p == NULL)
	{
		printf("HelloWorld!\n");
	}
	char *tt = test(a);
	printf("tt = %s\n", tt);
	return 0;
}
