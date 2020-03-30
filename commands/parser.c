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


struct parsed_part *parse_all_input(char *buf)
{
    struct parsed_part *parse = new_parsed_part();
    char *separator= ">";
    char *parsed;

    parsed = strtok(buf, separator);
    struct parsed_part *temp = parse;

    while(parsed != NULL)
    {
        temp->buf = calloc(strlen(parsed), sizeof(char));
        strcpy(temp->buf, parsed);
        //temp->buf = parsed;
        temp->next = new_parsed_part();
        temp = temp->next;
        parsed = strtok(NULL, separator);
    }
    temp = NULL;

    temp = parse;
    while(temp->buf)
    {
        char *t = calloc(strlen(temp->buf), sizeof(char));
        strcpy(t, temp->buf);
        parse_args(t, &temp->argc, temp->args);
        temp = temp->next;
        free(t);
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
        args->value = calloc(strlen(parsed), sizeof(char));
        strcpy(args->value, parsed);
        //args->value = parsed;
        args->next = new_parsed_arg();
        args = args->next;
        *argc += 1;
        parsed = strtok(NULL, separator);
    }
    args = NULL;
}

char** parse_part_to_arg(char *buf)
{
    int alloc = 8;
    char **parse = calloc(alloc, sizeof(char *));
    if (parse == NULL)
        errx(EXIT_FAILURE, "Error trying to allocate memory for the parse_input");

    int args = 0;
    char *separator = " ";
    char *parsed;

    char *temp = calloc(strlen(buf), sizeof(char));
    strcpy(temp, buf);

    parsed = strtok(temp, separator);
    while (parsed != NULL)
    {
        if (args == alloc - 1)
        {
            alloc *= 2;
            parse = realloc(parse, alloc * sizeof(char *));
            if (parse == NULL)
                errx(EXIT_FAILURE, "Error trying to realloc memory for the parse_input");
        }
        parse[args] = calloc(strlen(parsed), sizeof(char));
        strcpy(parse[args], parsed);
        //parse[args] = parsed;
        args++;
        parsed = strtok(NULL, separator);
    }
    parse[args] = NULL;

    free(temp);
    return parse;
}
