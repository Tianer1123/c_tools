#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <iconv.h>
#define OUTLEN 255	

int urldecode(char *str, int len)
{
	char *dest = str;
	char *data = str;

	int value;
	int c;

	while (len--) {
		if (*data == '+') {
			*dest = ' ';
		}
		else if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1))
				&& isxdigit((int) *(data + 2)))
		{

			c = ((unsigned char *)(data+1))[0];
			if (isupper(c))
				c = tolower(c);
			value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;
			c = ((unsigned char *)(data+1))[1];
			if (isupper(c))
				c = tolower(c);
			value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

			*dest = (char)value ;
			data += 2;
			len -= 2;
		} else {
			*dest = *data;
		}
		data++;
		dest++;
	}
	*dest = '\0';
	return dest - str;
}

//代码转换:从一种编码转为另一种编码
int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
{
	iconv_t cd;
	int rc;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset,from_charset);
	if (cd==0) return -1;
	memset(outbuf,0,outlen);
	if (iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
	iconv_close(cd);
	return 0;
} 


//GB2312码转为UNICODE码
int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
} 

int main(int argc, char** argv)
{
	char src[] = "dfsabuxing%CE%D2%D2%BB%B6%A8%D2%AA%B6%A5%C6%F0%C0%B4fdsafdsfffd";
	size_t len = strlen(src);
	
	urldecode(src, len);

	printf("%s\n", src);
	char out[OUTLEN] = {'\0'};
	int rc = 0;
	rc = g2u(src, len, out, OUTLEN);

	printf("%s\n", out);


	
	return 0;
}
