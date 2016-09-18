#include <stdio.h>
#include <string.h>

/* 
 * 选择排序 
 *
 *1. 找到数组中最小的元素. 
 *2. 将它与数组的第一个元素交换位置.
 *3. 在剩下的元素中找到最小元素.
 *4. 与第二个元素交换位置.
 *5. 循环知道整个数组排序.
 *
 * */
void selection_sort(char *src, int size)
{
	int i = 0;
	int j = 0;
	int min;
	char tmp;

	
	for (i = 0; i < size; i++) {
		min = i;
		for (j = i + 1; j < size; j++) {
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
