#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#define PATH_SIZE 512
#define FILE_PATH_SIZE 1024
#define MAX_LINE_SIZE FILE_PATH_SIZE

int get_line_num(char *path)
{
	FILE *fp = NULL;
	int num = 0;
	char line[MAX_LINE_SIZE] = {'\0'};

	fp = fopen(path, "r");
	if (!fp) {
		printf("error: open file %s failed!\n", path);
		return 0;
	}

	while (fgets(line, MAX_LINE_SIZE, fp)) {
		num++;
	}

	fclose(fp);

	return num;
}

void get_num(char *path, int *line_num)
{
	char tmp_path[FILE_PATH_SIZE] = {'\0'};
	DIR *dir = NULL;
	struct dirent *entry = NULL;
	struct stat st;

	if ((dir = opendir(path)) == NULL) {
		perror("opendir");
		return;
	}

	while((entry = readdir(dir))) {

		snprintf(tmp_path, FILE_PATH_SIZE, "%s/%s", path, entry->d_name);

		if (lstat(tmp_path, &st) == -1) {
			perror("lstat");
			continue;
		}

		if ((st.st_mode & S_IFMT) == S_IFDIR) {
			/* 过滤掉. ..目录 */
			if (!(memcmp(entry->d_name, ".", 1)) || !(memcmp(entry->d_name, "..", 2))) {
				continue;
			}
			get_num(tmp_path, line_num);
		} else {
			/* 以.开头的隐藏文件不统计 */
			if (!(memcmp(entry->d_name, ".", 1))) {
				continue;
			}
			*line_num += get_line_num(tmp_path);
		}
	}

	closedir(dir);
}

int main(int argc, char **argv)
{
	int line_num = 0;
	char path[PATH_SIZE] = {'\0'};
	if (argc < 2) {
		printf("USEAGE: ./a.out [path]!\n");
		exit(-1);
	}

	memcpy(path, argv[1], strlen(argv[1]));

	if (access(path, F_OK) != 0) {
		printf("path %s isn't exist!\n", path);
		exit(-1);
	}

	get_num(path, &line_num);

	printf("all line num is : %d\n", line_num);

	return 0;
}
