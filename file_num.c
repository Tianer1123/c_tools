#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define PATH_SIZE 512
#define FILE_PATH_SIZE 1024

int get_line_num(char *tmp_path)
{
	return 0;
}

void get_num(char *path, int *line_number)
{
	char tmp_path[FILE_PATH_SIZE] = {'\0'};
	int num = 0;
	DIR *dir = NULL;
	struct dirent *entry = NULL;
	struct stat st;

	if ((dir = opendir(path)) == NULL) {
		printf("open dir failed!\n");
		return;
	}

	while((entry = readdir(dir))) {
		if (lstat(entry->d_name, &st) == -1) {
			printf("can't get the stat of file %s.\n", entry->d_name);
			continue;
		}

		if ((st.st_mode & S_IFMT) == S_IFDIR) {
			/* . ..目录过滤掉 */
			if (!(memcmp(entry->d_name, ".", 1)) || !(memcmp(entry->d_name, "..", 2))) {
				continue;
			}
			printf("----%s is a dir!\n", entry->d_name);
		} else {
			/* 以.开头的隐藏文件不统计 */
			if (!(memcmp(entry->d_name, ".", 1))) {
				continue;
			}
			num = get_line_num(tmp_path);
		}
		printf("file name:%s\n", entry->d_name);
	}

	closedir(dir);
}

int main(int argc, char **argv)
{
	int line_nums = 0;
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

	get_num(path, &line_nums);

	return 0;
}
