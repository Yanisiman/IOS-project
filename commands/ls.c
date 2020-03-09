#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <string.h>

void listdir(char *pwd, int indent, int recursive, int info)
{
	DIR *dir;
	struct dirent *entry;

	if (!(dir = opendir(pwd)))
		return;

	while((entry = readdir(dir)) != NULL)
	{
		struct stat file_stat;
		char abspath[512];
		sprintf(abspath,"%s/%s", pwd, entry->d_name);
		if (stat(abspath, &file_stat) < 0)
			err(EXIT_FAILURE, "Error with stats of the file %s", entry->d_name);

		char perms[10];
		perms[0] = (S_ISDIR(file_stat.st_mode)) ? 'd' : '-';
		perms[1] = (file_stat.st_mode & S_IRUSR) ? 'r' : '-';
		perms[2] = (file_stat.st_mode & S_IWUSR) ? 'w' : '-';
		perms[3] = (file_stat.st_mode & S_IXUSR) ? 'x' : '-';
		perms[4] = (file_stat.st_mode & S_IRGRP) ? 'r' : '-';
		perms[5] = (file_stat.st_mode & S_IWGRP) ? 'w' : '-';
		perms[6] = (file_stat.st_mode & S_IXGRP) ? 'x' : '-';
		perms[7] = (file_stat.st_mode & S_IROTH) ? 'r' : '-';
		perms[8] = (file_stat.st_mode & S_IWOTH) ? 'w' : '-';
		perms[9] = (file_stat.st_mode & S_IXOTH) ? 'x' : '-';

		if (entry->d_type == DT_DIR)
		{
			char path[512];
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			{
				printf("%*s%s[%s]\n", indent, "", info ? perms : "", entry->d_name);
				continue;
			}
			snprintf(path, sizeof(path), "%s/%s", pwd, entry->d_name);
			printf("%*s%s[%s]\n", indent, "", info ? perms : "", entry->d_name);
			if (recursive)
				listdir(path, indent + 2, recursive, info);
		}
		else
		{
			printf("%*s- %s %s\n", indent, "", info ? perms : "", entry->d_name);
		}

	}  
	closedir(dir);
}


void ls(int argc, char** argv)
{
	if (argc < 1 || argc > 3)
		err(EXIT_FAILURE, "Arguments given are not good");

	int recursive = 0, info = 0;
	char tmp[256];
	char *pwd = getcwd(tmp, sizeof(tmp));
	if (argc == 3)
	{
		pwd = argv[2];
	}

	if (argc > 1 && argv[1][0] == '-')
	{
		if (strcmp(argv[1],"-l") == 0)
			info = 1;
		if (strcmp(argv[1],"-R") == 0)
			recursive = 1;
		if (strcmp(argv[1],"-lR") == 0 || strcmp(argv[1],"-Rl") == 0)
		{
			recursive = 1;
			info = 1;
		}
	}
	else if (argc == 2)
	{
		pwd = argv[1];
	}

	listdir(pwd, 0, recursive, info);

}

void simple_ls()
{
	char tmp[256];
	char *pwd = getcwd(tmp, sizeof(tmp));

	listdir(pwd, 0, 0, 0);

}
