#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stuinfo.h"

struct stuinfo *init_stuinfo(void)
{
	struct stuinfo *tstuinfo;
	tstuinfo = (struct stuinfo *)malloc(sizeof(struct stuinfo));
	memset(tstuinfo, 0, sizeof(struct stuinfo));

	INIT_LIST_HEAD(&tstuinfo->list);
	return tstuinfo;
}


int insertstuinfo(char *name, int age, char *addr, struct list_head *head)
{
	struct stuinfo *ttmpstuinfo;
	ttmpstuinfo = (struct stuinfo *)malloc(sizeof(struct stuinfo));
	if (ttmpstuinfo == NULL)
		return 1;

	strcpy(ttmpstuinfo->name, name);
	ttmpstuinfo->age = age;
	strcpy(ttmpstuinfo->addr, addr);

	list_add_tail(&ttmpstuinfo->list, head);
	return 0;
}

void iterator_stuinfolist(struct list_head *head)
{
	struct list_head *pos;
	struct stuinfo *tmp;

	list_for_each(pos, head) {
		tmp = list_entry(pos, struct stuinfo, list);
		printf("name:%s\tage:%d\taddr:%s\n",
				tmp->name, tmp->age, tmp->addr);
	}
}

struct stuinfo *find_stuinfo_by_name(char *name, struct list_head *head)
{
	struct list_head *pos;
	struct stuinfo *tmp;

	list_for_each(pos, head) {
		tmp = list_entry(pos, struct stuinfo, list);
		if (strcmp(tmp->name, name) == 0) {
			return tmp;
		}
	}
	return NULL;
}

void del_stuinfo(char *name, struct list_head *head)
{
	struct list_head *pos;
	struct stuinfo *tmp;

	list_for_each(pos, head) {
		tmp = list_entry(pos, struct stuinfo, list);
		if (strcmp(tmp->name, name) == 0) {
			list_del(pos);

			free(tmp);
			tmp = NULL;

			return;
		}
	}
}


void del_all_stu(struct list_head *head)
{
    struct list_head *pos, *next;
    struct stuinfo *tmp;

    list_for_each(pos, head) {
        next = pos->next;

        tmp = list_entry(pos, struct stuinfo, list);

        if (!list_empty_careful(head)) {
            list_del(pos);
            free(tmp);
            tmp = NULL;
            pos = next;
        } else {
            printf("list is empty!\n");
        }
    }

}
