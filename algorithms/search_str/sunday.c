#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>


#define LETTER_MAX_LEN 256

static int sunday(const char *src, const char *des)
{
	int i, pos = 0;
	int len_s, len_d;
	int alphabet[LETTER_MAX_LEN] = {0};

	if(src == NULL || des == NULL)
		return -1;

	len_s = strlen(src);
	len_d = strlen(des);

	for(i = 0; i < LETTER_MAX_LEN; i++)
		alphabet[i] = len_d;

	for(i = 0; i < len_d; i++)
		alphabet[des[i]] = len_d - i - 1;

	for(pos = 1; pos <= len_s - len_d; ) {
		for(i = pos - 1; i - pos + 1 < len_d; i++) {
			if(src[i] != des[i - pos + 1]) 
				break;
		}

		if((i - pos + 1) == len_d)
			return pos;
		else
			pos += alphabet[src[pos + len_d - 1]] + 1;
	}

	return -1;
}

int main(int argc, char **argv)
{
	const char *src = "Hello World!";
	const char *dst ="o W";
	int index = sunday(src, dst);
	if (index != -1) {
		printf("%d\n", index);
	}
	return 0;
}
