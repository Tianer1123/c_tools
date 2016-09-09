#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "memory.h"

struct MemoryPool mpool;

struct test
{
	int b[10];
};

int main(int argc, char **argv)
{
	u16 unitSize = sizeof(struct test);
	create_memory_pool(&mpool, unitSize, 0, 65535);

	struct test *t1;
	int num = 10000000;
        long timeuse;
        int i = 0;
#if 1
        struct timeval start, end;
        gettimeofday(&start, NULL);
	for (i = 0; i < num; i++) {
		t1 = mem_malloc(&mpool);
	}
	gettimeofday(&end, NULL);
	timeuse = 1000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000;
	printf("mem_malloc used time:%ld\n", timeuse);
	destory_memory_pool(&mpool);
#endif

#if 1
        struct timeval start1, end1;
        gettimeofday(&start1, NULL);
	for (i = 0; i < num; i++) {
		t1 = malloc(sizeof(struct test));
	}
	gettimeofday(&end1, NULL);
	timeuse = 1000*(end1.tv_sec - start1.tv_sec) + (end1.tv_usec - start1.tv_usec)/1000;
	printf("malloc used time:%ld\n", timeuse);
#endif
                
	return 0;
}
