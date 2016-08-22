#include <stdio.h>
#include <string.h>

void getNext(const char *p, int *next)
{
	int i, k;  //i为下标,k为next[i]的值. i只递增,k回溯
	int slen = strlen(p); //要查找的串的长度.
	next[0] = 0; //p[0]的next值为0;

	for (i = 1, k = 0; i < slen; ++i) {
		while (k != 0 && p[i] != p[k])
			k = next[k - 1];        //KMP算法核心部分

		if (p[i] == p[k])
			k++;

		next[i] = k;
	}
}

int kmp(const char *t, const char *p, int *next)
{
	int i, q;
	int tlen = strlen(t);
	int plen = strlen(p);

	getNext(p, next);

	for (i = 0, q = 0; i < tlen; i++) {
		while (q > 0 && p[q] != t[i])
			q = next[q - 1];

		if (t[i] == p[q])
			q++;

		if (q == plen)
			return (i - q + 1);
	}
	return 0;
}

int main(void)
{
	int i, len;
	int next[20]={0};
	char T[] = "abcdabcbabcdabdsss";
	char P[] = "abcdabd";
	printf("%s\n",T);
	printf("%s\n",P );
	int res = kmp(T, P, next);
	len = strlen(P);
	printf("index:%d\n", res);
	for (i = 0; i < len; ++i)
	{
		printf("%d ",next[i]);
	}
	printf("\n");
	return 0;
}
