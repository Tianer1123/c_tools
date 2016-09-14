#include <stdio.h>
#include <string.h>

/* 
 * 选择排序 
 *
 * 1. 遍历查找最小的数字与第一个交换位置.
 * 2. 从第二个开始遍历查找最小的与第二个互换位置.
 * 3. 循环.
 *
 * */
void selection_sort(char *src, int size)
{
	int i = 0;
	int j = 0;
	int min;
	char tmp;

	
	for (i = 0; i < size - 1; i++) {
		min = i;
		for (j = i; j < size; j++) {
			if (src[min] > src[j]) {
				min = j;
			}
		}
		tmp = src[min];
		src[min] = src[i];
		src[i] = tmp;
		printf("第%d次交换后的值：%s\n", i, src);
	}
}


/* test */
int main(int argc, char **argv)
{
	char str[] = {'S', 'O', 'R', 'T', 'E', 'X', 'A', 'M', 'P', 'L', 'E'};
	selection_sort(str, strlen(str));
	return 0;
}
