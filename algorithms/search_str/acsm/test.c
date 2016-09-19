#include <stdio.h>
#include "acsmx.h"

int printMatch(ACSM_PATTERN * pattern,ACSM_PATTERN * mlist, int nline,int index)
{

	if (*(int16_t *)mlist->patrn == 0x0a0d) {
		return 0;     
	}
	//return 1;
	/* Count the Each Match Pattern */
	ACSM_PATTERN *temp = pattern;
	for (;temp!=NULL;temp=temp->next)
	{
		if (!strcmp(temp->patrn,mlist->patrn)) //strcmp succeed return 0,So here use "!" operation
		{
			temp->nmatch++;
		}

	}

	if(mlist->nocase)
		fprintf (stdout, "Match KeyWord %s at %d line %d char\n", mlist->patrn,nline,index);
	else
		fprintf (stdout, "Match KeyWord %s at %d line %d char\n", mlist->casepatrn,nline,index);

	return 1;
}
				

int main(int argc, char **argv)
{
	int nocase = 0;
	char *main_str = "abcdefwhersacdsef";
	char *p1 = "she";
	char *p2 = "his";
	char *p3 = "hers";
	ACSM_STRUCT * acsm;
	acsm = acsmNew();
	acsmAddPattern(acsm, p1, strlen(p1), nocase);
	acsmAddPattern(acsm, p2, strlen(p2), nocase);
	acsmAddPattern(acsm, p3, strlen(p3), nocase);

	acsmCompile(acsm);

	acsmSearch(acsm, main_str, strlen(main_str), printMatch);

	return 0;
}
