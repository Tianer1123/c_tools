#include "stuinfo.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc,char* argv[])
{  
	struct stuinfo *tstuinfo;
	struct stuinfo *findstuinfo;
	tstuinfo = init_stuinfo();
	if (insertstuinfo("xiaoli",18,"beijing",&tstuinfo->list))
	{  
		printf("insert error!\n");
		exit(0);
	}  
	if (insertstuinfo("xiaoming",14,"guangzhou",&tstuinfo->list))  
	{  
		printf("insert error!\n");  
		exit(0);  
	}  
	if (insertstuinfo("xiaobai",21,"shanghai",&tstuinfo->list))  
	{  
		printf("insert error!\n");  
		exit(0);  
	}  
	if (insertstuinfo("xiaohei",25,"hubei",&tstuinfo->list))  
	{  
		printf("insert error!\n");  
		exit(0);  
	}  
	iterator_stuinfolist(&tstuinfo->list);  
	findstuinfo = find_stuinfo_by_name("xiaobai",&tstuinfo->list);  
	if(findstuinfo == NULL)  
		printf("not find the name of xiaobai\n");  
	else  
	{  
		printf("findstuinfo->name:%s \t findstuinfo->age:%d \t findstuinfo->addr :%s\n",findstuinfo->name,findstuinfo->age,findstuinfo->addr);  

		del_stuinfo(findstuinfo->name,&tstuinfo->list);  
	}  
	iterator_stuinfolist(&tstuinfo->list);
    del_all_stu(&tstuinfo->list);
	return 0;  
}  


