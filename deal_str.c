#include <stdio.h>
#include <string.h>

//char* attachment = "fujian1\02100;fujian2\02200;fujian3\02300";
char* attachment = "fujian1\02100;fujian2\02200";
//char* attachment = "fujian1\02100";
//char *attachment = "";
char attach_num[1] = {'\0'};
char attach1[200] = {'\0'};
char attach2[200] = {'\0'};
char attach3[200] = {'\0'};

char* deal_attach(char *attach, char* attach_tmp)
{
	char* tmp = attach;
	unsigned long i = 0;
	if(strlen(attach) == 0)
	{
		return "null-attach";
	}
#if 0
	char* endpos = strstr(attach, p);
	printf("endpos = %s\n", endpos);
	//memcpy(attach_tmp, attach, startpos - endpos);
	
#endif
#if 1
	for( ; i < strlen(tmp); i++)
	{
		if(tmp[i] == '\\')
		{
			return attach_tmp;
		}
		attach_tmp[i] = tmp[i];
	}
#endif
	//return attach_tmp;
	return attach_tmp;
}


int my_strtok(char *attachment)
{
	int len = strlen(attachment);
	int i   = 0;
	int j   = 0;
	int num = 0;

	for( ; i < len; i++)
	{
		if(attachment[i] == ';')
		{
			j = 0;
			num++;
			continue;
		}
		else
		{
			if(num == 0)
			{
				attach1[j] = attachment[i];
			}
			else if(num == 1)
			{
				attach2[j] = attachment[i];
			}
			else if(num == 2)
			{
				attach3[j] = attachment[i];
			}
			j++;
		}
	}
	return num;
}
#if 0
int deal_attachment(char* attachment)
{
	int num = 0;

	if(strlen(attachment) == 0)
	{
		attach_num[0] = '0';
		strcpy(attach1, "null-attach");
		strcpy(attach2, "null-attach");
		strcpy(attach3, "null-attach");
		return num;
	}

}
#endif

int deal_attachment(char* attachment)
{
	int num = 0;
	if(strlen(attachment) == 0)
	{
		attach_num[0] = '0';
		strcpy(attach1, "null-attach");
		strcpy(attach2, "null-attach");
		strcpy(attach3, "null-attach");
		return num;
	}
	printf("attachment = %s\n", attachment);
	num = my_strtok(attachment) + 1;
	return num;
}


int main(int argc, char** argv)
{
	int num = 0;
	char attach_tmp[200] = {'\0'};

	num = deal_attachment(attachment);
	strcpy(attach1, deal_attach(attach1, attach_tmp));
	strcpy(attach2, deal_attach(attach2, attach_tmp));
	strcpy(attach3, deal_attach(attach3, attach_tmp));

	printf("attach1 = %s\n", attach1);
	printf("attach2 = %s\n", attach2);
	printf("attach3 = %s\n", attach3);
	printf("num = %d\n", num);
	return 0;
}
