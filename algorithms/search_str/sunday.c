#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int status;
#define MAXSIZE 100
#define ERROR 0
#define TRUE 1

//初始化数据
status InitData(char **source,char **target){
		char ch;
		int i = 0;
		(*source) = (char *)malloc(sizeof(char) * MAXSIZE);
		(*target) = (char *)malloc(sizeof(char) *MAXSIZE);

		if((*source)==NULL || (*target) ==NULL){
				printf("初始化错误...\n");
				return ERROR;
		}
		//输入字符串
		printf("请输入字符串，以#结束\n");
		while((ch = getchar())!='#'){
				(*source)[i++] = ch;
				(*source)[i] = '\0';
		}
		getchar();  //清除缓冲区回车键
		i = 0;  //重置
		printf("请输入搜索词，以#结束\n");
		while((ch = getchar()) !='#'){
				(*target)[i++] = ch;
				(*target)[i]='\0';
		}
}
//找出temp在target的位置
int FindIndex(char *target,char temp){
		int i= 0;
		if(target ==NULL){
				printf("搜索词为空...\n");
				exit(-1); //异常退出
		}
		for(i = strlen(target) -1;i>=0;i--){
				if(target[i] == temp){
						return i;
				}
		}
		return -1;  //未找到字符匹配位置

}
//sunday算法
status Sunday(char *source,char *target){

		int i= 0,j = 0,srclen = strlen(source),tarlen=strlen(target);
		int temp  =0,index = -1;
		if(source ==NULL || target ==NULL){
				printf("请初始化...\n");
				return ERROR;
		}

		while(i<srclen){  //循环条件
				if(source[i] == target[j]){
						if(j==tarlen-1){
								printf("匹配成功...位置：%d\n", i - j + 1);
								return TRUE;
						}
						i++;j++;
				}else{  //发现不相等位置
						temp = tarlen - j + i;  //字符串后面的第一个字符位置
						index = FindIndex(target,source[temp]);
						if(index==-1){ //未找到位置，后移
								i = temp+1;
								j = 0;
						}else{  //找到位置
								i = temp-index;
								j = 0;
						}
				}
		}
		printf("匹配失败..\n");
		return ERROR;
}

void main(){

		char *source,*target;

		InitData(&source,&target);
		Sunday(source,target);
}
