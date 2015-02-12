#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "gb2312_unicode_table.h"
#include "unicode_gb2312_table.h"
#include "encoded.h"

/***
 * =========================
 * test使用的头文件
 *
 * */
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
/*=========================*/

int GetUtf8ByteNumForWord(unsigned char firstCh)
{
	unsigned char temp = 0x80;
	int num = 0;

	while(temp & firstCh)
	{
		num++;
		temp = (temp >> 1);
	}
	return num;
}

#define CODE_TABLE_SIZE 7445
/*这个表是死的，所以就直接用宏表示长度，不用每次都用size，不过这样可能对移植性不好。*/
/*unicode --> gb2312*/
unsigned short SearchCodeTable(unsigned short unicodeKey)
{
	int first = 0;
	int end = CODE_TABLE_SIZE - 1;
	int mid = 0;

	while (first <= end)
	{
		mid = (first + end) / 2;

		if (code_table[mid].unicode == unicodeKey)
		{
			return code_table[mid].gb2312;
		}
		else if (code_table[mid].unicode > unicodeKey)
		{
			end = mid - 1;
		}
		else 
		{
			first = mid + 1;
		}
	}
	return 0;
}

/*gb2312 --> unicode*/
unsigned short SearchCodeTable2(unsigned short gbKey)
{
	int first = 0;
	int end = CODE_TABLE_SIZE - 1;
	int mid = 0;

	while (first <= end)
	{
		mid = (first + end) / 2;

		if (code_table2[mid].gb2312 == gbKey)
		{
			return code_table2[mid].unicode;
		}
		else if (code_table2[mid].gb2312 > gbKey)
		{
			end = mid - 1;
		}
		else 
		{
			first = mid + 1;
		}
	}
	return 0;
}


/*将len个字节的UTF8格式的转换成GB2312格式存放在temp预先申请好的缓冲区中*/
void Utf8ToGb2312(unsigned const char* utf8, int len, unsigned char *temp)
{

	int byteCount = 0;
	int i = 0;
	int j = 0;

	unsigned short unicodeKey = 0;
    unsigned short gbKey = 0;


	/*循环解析*/
	while (i < len)
	{   
		unsigned char u8 = utf8[i];
		int num = GetUtf8ByteNumForWord(u8);
		switch(num)
		{
			case 0:
				temp[j] = utf8[i];
				byteCount = 1;
				break;

			case 2:
				temp[j] = utf8[i];
				temp[j + 1] = utf8[i + 1];
				byteCount = 2;
				break;

			case 3:
				/*这里就开始进行UTF8->Unicode*/
				temp[j + 1] = ((utf8[i] & 0x0F) << 4) | ((utf8[i + 1] >> 2) & 0x0F);
				temp[j] = ((utf8[i + 1] & 0x03) << 6) + (utf8[i + 2] & 0x3F);

				/*取得Unicode的值*/
				memcpy(&unicodeKey, (temp + j), 2);

				/*根据这个值查表取得对应的GB2312的值*/
				gbKey = SearchCodeTable(unicodeKey);

				if (gbKey != 0)
				{
					/*here change the byte*/
					/*不为0表示搜索到，将高低两个字节调换调成我要的形式*/
					gbKey = ntohs(gbKey);
					//gbKey = (gbKey >> 8) | (gbKey << 8);
					memcpy((temp + j), &gbKey, 2);
				}

				byteCount = 3;
				break;

			case 4:
				byteCount = 4;
				break;
			case 5:
				byteCount = 5;
				break;
			case 6:
				byteCount = 6;
				break;

			default:
				printf("the len is more than 6\n");
				break;    
		}

		i += byteCount;
		if (byteCount == 1)
		{
			j++;
		}
		else
		{
			j += 2;
		}
	}
}

void Gb2312ToUft8(unsigned const char* gb, int len, unsigned char* tmp)
{
	int i = 0;
	int j = 0;

	unsigned short gbKey;
	unsigned int unicodeKey;
	while(i < len)
	{
		memset(&gbKey, 0, sizeof(unsigned short));
		memcpy(&gbKey, (gb + i), 1);
		if(!(gbKey & 0x80))  //如果是英文字符
		{
			*(tmp+j) = gbKey;
			i++;
			j++;
		}
		else
		{
			memset(&gbKey, 0, sizeof(unsigned short));
			memcpy(&gbKey, (gb + i), 2);
			gbKey = ntohs(gbKey);

			//printf("gbKey:%x\t", gbKey);
			unicodeKey = SearchCodeTable2(gbKey);
			//printf("unicodeKey:%x\n", unicodeKey);
			if(unicodeKey <= 0x0000007F) //one byte
			{
				*(tmp+j) = (unicodeKey & 0x7F);
				j++;
			}
			else if(unicodeKey >= 0x00000080 && unicodeKey <= 0x000007FF) //two bytes
			{
				*(tmp+j+1) = (unicodeKey & 0x3f) | 0x80;
				*(tmp+j)   = ((unicodeKey >> 6) & 0x1F) | 0xC0;
				j += 2;

			}
			else if(unicodeKey >= 0x00000800 && unicodeKey <= 0x0010FFFF)//three bytes
			{
				*(tmp+j+2) = (unicodeKey & 0x3f) | 0x80;
				*(tmp+j+1) = ((unicodeKey >> 6) & 0x3F) | 0x80;
				*(tmp+j)   = ((unicodeKey >> 12) & 0x0F) | 0xE0;
				j += 3;
			}
			else if ( unicodeKey >= 0x00010000 && unicodeKey <= 0x001FFFFF )  //four bytes
			{  

				*(tmp+j+3) = (unicodeKey & 0x3F) | 0x80;  
				*(tmp+j+2) = ((unicodeKey >>  6) & 0x3F) | 0x80;  
				*(tmp+j+1) = ((unicodeKey >> 12) & 0x3F) | 0x80;  
				*(tmp+j)   = ((unicodeKey >> 18) & 0x07) | 0xF0;  
				j += 4;
			}  
			else if ( unicodeKey >= 0x00200000 && unicodeKey <= 0x03FFFFFF )  //five bytes
			{  

				*(tmp+j+4) = (unicodeKey & 0x3F) | 0x80;  
				*(tmp+j+3) = ((unicodeKey >>  6) & 0x3F) | 0x80;  
				*(tmp+j+2) = ((unicodeKey >> 12) & 0x3F) | 0x80;  
				*(tmp+j+1) = ((unicodeKey >> 18) & 0x3F) | 0x80;  
				*(tmp+j)   = ((unicodeKey >> 24) & 0x03) | 0xF8;  
				j += 5;
			}  
			else if ( unicodeKey >= 0x04000000 && unicodeKey <= 0x7FFFFFFF )  //six bytes
			{  

				*(tmp+j+5) = (unicodeKey & 0x3F) | 0x80;  
				*(tmp+j+4) = ((unicodeKey >>  6) & 0x3F) | 0x80;  
				*(tmp+j+3) = ((unicodeKey >> 12) & 0x3F) | 0x80;  
				*(tmp+j+2) = ((unicodeKey >> 18) & 0x3F) | 0x80;  
				*(tmp+j+1) = ((unicodeKey >> 24) & 0x3F) | 0x80;  
				*(tmp+j)     = ((unicodeKey >> 30) & 0x01) | 0xFC;  
				j += 6;
			}
			else
			{
				printf("bytes too long\n");
			}
			i += 2;
		}
	}
}
#if 0
int main(void)
{
	/*unsigned char buf[20] = "aff啊崩fds";*/
	unsigned char buf[100] = {'\0'};
	int fd = open("./test.txt", O_CREAT|O_RDWR|O_APPEND, 0666);//test.txt为gb2312
	read(fd, buf, 100);
	close(fd);
	unsigned char des[300] = {'\0'};
	int len = strlen(buf);
	printf("%s\n", buf);
	int j = 0;
	for(; j < len; j++)
	{
		printf("0x%x ", buf[j]);
	}
	printf("\n");


	/*Utf8ToGb2312(buf, len ,des);*/
	Gb2312ToUft8(buf, len, des);
	printf("%s\n", des);
	/*test*/
	return 0;
}
#endif
