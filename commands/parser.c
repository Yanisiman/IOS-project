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
    parse->buf = NULL;

    return parse;
}

void free_parsed_part(struct parsed_part *parse)
{
    if (parse == NULL)
        return;
    free_parsed_part(parse->next);
    if (parse->buf != NULL)
        free(parse->buf);
    free_parsed_arg(parse->args);
    free(parse);
}

void free_parsed_arg(struct parsed_arg *parse)
{
    if (parse == NULL)
        return;
    free_parsed_arg(parse->next);
    if (parse->value != NULL)
        free(parse->value);
    free(parse);
}

void free_parse_command(char **parse)
{
    for (int i = 0; parse[i]; i++)
        free(parse[i]);
    free(parse);
}


struct parsed_part *parse_all_input(char *buf, char* separator)
{
    struct parsed_part *parse = new_parsed_part();
    char *parsed;

    parsed = strtok(buf, separator);
    struct parsed_part *temp = parse;

    while(parsed != NULL)
    {
        temp->buf = calloc(strlen(parsed) + 1, sizeof(char));
        strcpy(temp->buf, parsed);
        temp->next = new_parsed_part();
        temp = temp->next;
        parsed = strtok(NULL, separator);
    }
    temp = NULL;

    temp = parse;
    char *sep = " ";
    while(temp->buf)
    {
        char *t = calloc(strlen(temp->buf) + 1, sizeof(char));
        strcpy(t, temp->buf);
        parse_args(t, &temp->argc, temp->args, sep);
        temp = temp->next;
        free(t);
    }

    return parse;
}

void parse_args(char *buf, int *argc, struct parsed_arg *args, char* separator)
{
    char *parsed;
    parsed = strtok(buf, separator);

    while(parsed)
    {
        args->value = calloc(strlen(parsed) + 1, sizeof(char));
        strcpy(args->value, parsed);
        args->next = new_parsed_arg();
        args = args->next;
        *argc += 1;
        parsed = strtok(NULL, separator);
    }
    args = NULL;
}

char** parse_part_to_arg(struct parsed_part *p, char *separator, int *args)
{
    int alloc = 8;
    char **parse = calloc(alloc, sizeof(char *));
    if (parse == NULL)
        errx(EXIT_FAILURE, "Error trying to allocate memory for the parse_input");

    char *parsed;
    struct parsed_part *tp = p;
    while(tp->buf)
    {
        int i = 0;
        int j = 0;
        char *buf = tp->buf;
        char *temp = calloc(strlen(buf) + 1, sizeof(char));
        strcpy(temp, buf);

        parsed = strtok(temp, separator);
        while (parsed != NULL)
        {
            if (*args == alloc - 1)
            {
                alloc *= 2;
                parse = realloc(parse, alloc * sizeof(char *));
                if (parse == NULL)
                    errx(EXIT_FAILURE, "Error trying to realloc memory for the parse_input");
            }
            if (*args == 0 || j >= 1 || (j == 0 && i > 1))
            {
                parse[*args] = calloc(strlen(parsed) + 1, sizeof(char));
                strcpy(parse[*args], parsed);
                *args += 1;
            }
            j++;
            parsed = strtok(NULL, separator);
        }

        free(temp);
        tp = tp->next;
        i++;
    }
    parse[*args] = NULL;

    return parse;
}
