#include <stdio.h>
#include "memory.h"

#define GROWSIZE 4
struct MemoryPool mpool;

struct test
{
	int a;
	int b;
};

int main(int argc, char **argv)
{
	u16 unitSize = (u16)(sizeof(struct test) + 2);
	create_memory_pool(&mpool, unitSize, 0, GROWSIZE);

	struct test *t1 = mem_malloc(&mpool);
	struct test *t2 = mem_malloc(&mpool);
	printf("t1:%p\n", t1);
	printf("t2:%p\n", t2);
	mem_free(&mpool, t1);
	mem_free(&mpool, t2);
	destory_memory_pool(&mpool);
	return 0;
}
