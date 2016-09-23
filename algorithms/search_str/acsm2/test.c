#include "acsmx2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct pattern_s
{
	uint8_t *pattern;
	uint8_t  identity;
	uint16_t nocase;
	uint16_t len;
	uint16_t id;
	uint16_t index;
};

typedef struct target_str {
	uint8_t *str;
	unsigned int tlen;
	uint8_t  mask;
}T;

typedef struct match_pattern {
	struct pattern_s feature;
	struct pattern_s key;
	void (*func)(struct target_str *, struct match_pattern *);
} MP;

void getValue(T *target, MP *mp) {
	unsigned char value[64] = {'\0'};
	char *tmp = target->str + mp->key.index + mp->key.len;
	int len = target->tlen - mp->key.index - mp->key.len;
	if (len < 0) return;

	int i = 0;
	for (; i < len; i++) {
		if (*tmp == ',') break;
		value[i] = *tmp++;
	}

	printf("value:%s\n", value);
}

int myMatch(void *id, void *tree, int index, void *data, void *neg_list)
{
	MP *mp = (MP *)id;
	T *target = (T *)data;

	printf("target:%s\n", target->str + index);
	if (memcmp(target->str + index, mp->feature.pattern, mp->feature.len) == 0) {
		mp->feature.index = index;
		target->mask++;
		if (target->mask == 2)
			mp->func(target, mp);
	}
	else if (memcmp(target->str + index, mp->key.pattern, mp->key.len) == 0) {
		mp->key.index = index;
		target->mask++;
		if (target->mask == 2)
			mp->func(target, mp);
	}
	return 0;
}

void init_MP(MP *mp)
{
	mp->feature.pattern = "defg/ad?s*/";
	mp->feature.identity = 0;
	mp->feature.nocase = 1;
	mp->feature.len = strlen(mp->feature.pattern);
	mp->feature.id = 1001;
	mp->feature.index = 0;

	mp->key.pattern = "name=";
	mp->key.identity = 0;
	mp->key.nocase = 1;
	mp->key.len = strlen(mp->key.pattern);
	mp->key.id = 1002;
	mp->key.index = 0;

	mp->func = getValue;
}

void init_T(T *target)
{
	target->str = "abcdefg/ad?s*/afdsafjlk?name=123432,fsfdsefd";
	target->tlen = strlen(target->str);
	target->mask = 0;
}

int main(int argc, char **argv)
{
	MP mp;
	T target;

	init_MP(&mp);
	init_T(&target);

	ACSM_STRUCT2 *acsm;
	acsm = acsmNew2(NULL, NULL, NULL);

	acsmAddPattern2(acsm, mp.feature.pattern, mp.feature.len, mp.feature.nocase, 0, 0, 0, (void *)&mp, mp.feature.id);
	acsmAddPattern2(acsm, mp.key.pattern, mp.key.len, mp.key.nocase, 0, 0, 0, (void *)&mp, mp.key.id);

	acsmCompile2(acsm, NULL, NULL);

	int current_state = 0;
	acsmSearchAll2(acsm, target.str, target.tlen, myMatch, (void *)&target, &current_state);

	acsmFree2(acsm);

	return 0;
}

