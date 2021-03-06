#ifndef PARSER_H
#define PARSER_H

struct parsed_arg
{
    char* value;
    struct parsed_arg *next;
};

struct parsed_part
{
    int parts;
    int argc;
    int append;
    char *buf;
    struct parsed_arg *args;
    struct parsed_part *next;
};

struct parsed_part *new_parsed_part();

struct parsed_arg *new_parsed_arg();

void free_parsed_part(struct parsed_part *parse);

void free_parsed_arg(struct parsed_arg *parse);

void free_parse_command(char **parse);

struct parsed_part *parse_all_input(char *buf, char *separator);

void parse_args(char *buf, int *argc, struct parsed_arg *args, char *separator);

char** parse_part_to_arg(struct parsed_part *p, char *separator, int *args);

char **parse_redirections(char *buf, int *output, struct parsed_part **parsed, int *argc, int *fd);

struct parsed_part *parse_pipes(char *buf);

struct parsed_part *parse_and(char *buf);
#endif
