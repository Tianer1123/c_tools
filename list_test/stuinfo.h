#ifndef __LIST_STU__
#define __LIST_STU__

#include "list.h"

struct stuinfo {
	char name[30];
	int age;
	char addr[50];
	struct list_head list;
};

struct stuinfo *init_stuinfo(void);
int insertstuinfo(char *name, int age, char *addr, struct list_head *head);
void iterator_stuinfolist(struct list_head *head);
struct stuinfo *find_stuinfo_by_name(char *name, struct list_head *head);
void del_stuinfo(char *name, struct list_head *head);
void del_all_stu(struct list_head *head);

#endif //__LIST_STU__

