char *read_to_string(int fd);

char **string_to_lines(char *file, int *count);

char **find_pattern(char **lines, char *pattern, int *count, int k);

int grep(int argc, char **argv);
