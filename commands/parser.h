#ifndef PARSER_H
#define PARSER_H

struct parsed_arg
{
    char* value;
    struct parsed_arg *next;
};

struct parsed_part
{
    struct parsed_arg *args;
    int argc;
    struct parsed_part *next;
};

struct parsed_part *new_parsed_part();

struct parsed_arg *new_parsed_arg();

struct parsed_part *parse_struct_input(char *buf);

void parse_args(char *buf, int *argc, struct parsed_arg *args);

#endif
