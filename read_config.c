#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "sys_config.cfg"
#define FILE_PATH_LEN 32
#define LINE_SIZE 512
#define KV_SIZE 64

char portal_sip[32];

void mytrim(char *str)
{
	char *start, *end, c;
	int len;

	if ((len = strlen(str)) == 0) {
		return;
	}

	for (start = str; (c = *start); start++) {
		if (!isspace((int)c)) {
			break;
		}
	}

	for (end = str + len; (end > start) && (c = *(end - 1)); end--) {
		if (!isspace((int)c)) {
			break;
		}
	}

	memmove(str, start, (size_t)(end - start));
	str[(size_t)(end - start)] = '\0';
}


void set_key_value(char *key, char *value)
{
	if (strcmp(key, "server_ip") == 0) {
		memcpy(portal_sip, value, strlen(value));
	}

	if (strlen(portal_sip) == 0) {
		memcpy(portal_sip, "192.168.1.111", 13);
	}

}

static int is_space_line(char *line)
{
	return !strlen(line);
}


void parse_line_data(char *line, char *key, char *value)
{
	char *index = line;
	int i = 0;

	int line_len = strlen(line);

	if (line_len == 0) {
		return;
	}

	if (line_len > LINE_SIZE) {
		line_len = LINE_SIZE;
	}

	while (*index) {
		if (*index == '=') {
			index++;
			i++;

			break;
		}

		if (i >= KV_SIZE) {
			printf("key too long! max size is : %d !\n", KV_SIZE);
			return;
		}

		key[i] = line[i];

		i++;
		index++;
	}

	if (i >= line_len) {
		return;
	}

	memcpy(value, index, line_len - i);

	mytrim(key);
	mytrim(value);

	set_key_value(key, value);
}

void init_sys_config()
{
	char file_path[FILE_PATH_LEN] = {0};
	char line[LINE_SIZE] = {0};
	char key[KV_SIZE] = {0};
	char value[KV_SIZE] = {0};
	char *index = NULL;
	FILE *fp = NULL;
	int line_num = 0;

	snprintf(file_path, 32, "./%s", FILE_NAME);

	fp = fopen(file_path, "r");
	if (fp == NULL) {
		return;
	}

	while (fgets(line, LINE_SIZE, fp)) {
		index = line;

		if ((strlen(line) + 1) >= LINE_SIZE) {
			printf("Line greater than or equal to %u characters which is "
					"more than the parser is willing to handle.", LINE_SIZE);
			printf("line num is : %d\n", line_num);
		}

		while (isspace((int)*index)) {
			index++;
		}

		if ((*index == '#') || is_space_line(index)) {
			line_num++;
			continue;
		}

		parse_line_data(index, key, value);

		line_num++;
	}

	fclose(fp);
}

int main(int argc, char **argv)
{
	init_sys_config();
	return 0;
}
