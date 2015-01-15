#include <stdio.h>
#include <string.h>


struct a
{
	char *t1;
	char *t2;
};

struct a quanzu;

struct a *test(int x, struct a *tmp)
{
	printf("%d\n", x);
	tmp = &quanzu;
}

int main(int argc, char** argv)
{
	char *p = NULL;
	//char *p1 = "helloWorld!";
	struct a *tmp = NULL;
	quanzu.t1 = "Hello";
	quanzu.t2 = "World";

	test(1, tmp);
	printf("%s\n", tmp->t1);
	printf("%s\n", tmp->t2);
	//p = p1;
	free(tmp);
	if(p == NULL)
	{
		printf("p is NULL\n");
	}
	else
	{
		printf("%s\n", p);
	}
	return 0;
}	
