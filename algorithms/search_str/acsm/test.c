#include <stdio.h>
#include "acsmx.h"

typedef struct _pattern_struct
{
	int id;
	void *ptr;
	int len;
	void (*func)(void *ptr, int len);
}pattern_struct_t;

void parse_tagert(void *ptr, int len)
{
	printf("payload:%s\n", ptr);
	printf("payload_len:%d\n", len);
}


int deal_pattern (ACSM_PATTERN * pattern, ACSM_PATTERN * mlist, void *id, int index)
{
	pattern_struct_t *ps = (pattern_struct_t *)id;
	printf("patrn:%s, casepatrn:%s, index:%d, id:%d\n", mlist->patrn, mlist->casepatrn, index, ps->id);
	if (ps->func)
		ps->func(ps->ptr, ps->len);
	return 0;
}

int main(int argc, char **argv)
{
	int nocase = 0;
	char *main_str = "abcdefwhersacdsef";
	unsigned char *p1 = "she";
	unsigned char *p2 = "his";
	unsigned char *p3 = "hers";
	unsigned char *p4 = "dse";
	pattern_struct_t pt1;
	pattern_struct_t pt2;
	pattern_struct_t pt3;
	pattern_struct_t pt4;
	pt3.id = 1001;
	pt3.ptr = (void *)main_str;
	pt3.len = strlen(main_str);
	pt3.func = parse_tagert;

	ACSM_STRUCT * acsm;
	acsm = acsmNew();
	acsmAddPattern(acsm, p1, strlen(p1), (void *)&pt1, nocase);
	acsmAddPattern(acsm, p2, strlen(p2), (void *)&pt2, nocase);
	acsmAddPattern(acsm, p3, strlen(p3), (void *)&pt3, nocase);
	acsmAddPattern(acsm, p4, strlen(p4), (void *)&pt4, nocase);

	acsmCompile(acsm);

	acsmSearch(acsm, main_str, strlen(main_str), deal_pattern);

	acsmFree(acsm);
	return 0;
}
