#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
	char* p = NULL;

	p = "";
	if(strlen(p) == 0 || p == NULL)
	{
		printf("HelloWorld!\n");
	}
	return 0;
}
