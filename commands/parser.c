#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include "parser.h"

struct parsed_arg *new_parsed_arg()
{
    struct parsed_arg *arg = calloc(1, sizeof(struct parsed_arg));
    if (arg == NULL)
        err(EXIT_FAILURE, "Error of memory allocation for the parsed_arg calloc");

    return arg;
}


struct parsed_part *new_parsed_part()
{
    struct parsed_part *parse = malloc(sizeof(struct parsed_part));
    if (parse == NULL)
        err(EXIT_FAILURE, "Error of memory allocation for the parsed_part malloc");

    parse->args = new_parsed_arg();
    parse->next = NULL;
    parse->argc = 0;

    return parse;
}


struct parsed_part *parse_struct_input(char *buf)
{
    struct parsed_part *parse = new_parsed_part();
    char *separator= ">";
    char *parsed;

    parsed = strtok(buf, separator);
    struct parsed_part *temp = parse;

    while(parsed != NULL)
    {
        parse_args(parsed, &temp->argc, temp->args);
        temp->next = new_parsed_part();
        temp = temp->next;
        parsed = strtok(NULL, separator);
    }
    return parse;
}

void parse_args(char *buf, int *argc, struct parsed_arg *args)
{
    char *separator = " ";
    char *parsed;
    parsed = strtok(buf, separator);

    while(parsed)
    {
        args->value = parsed;
        args->next = new_parsed_arg();
        args = args->next;
        *argc++;
        parsed = strtok(NULL, separator);
    }
}
