#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <fcntl.h>
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
    parse->append = 0;
    parse->parts = 0;
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
        int k = strlen(parsed);
        temp->buf = calloc(k + 5, sizeof(char));
        strcpy(temp->buf, parsed);
        temp->next = new_parsed_part();
        parse->parts += 1;
        char *p = parsed + k + 1;
        if (p[0] == '>')
            temp->next->append = 1;
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


char **parse_redirections(char *buf, int *output, struct parsed_part **parsed, int *argc, int *fd)
{
    char *sep_redirection = ">";
    char *sep_args = " ";

    *parsed = parse_all_input(buf, sep_redirection);
    char **parse_command = parse_part_to_arg(*parsed, sep_args, argc);

    struct parsed_part *temp_parse = *parsed;
    struct parsed_part *prev;
    while(temp_parse->buf)
    {
        prev = temp_parse;
        temp_parse = temp_parse->next;
    }

    int flags;
    int append = 0;

    *output = dup(STDOUT_FILENO);
    if ((*parsed)->next->buf)
    {
        if (prev->append == 1)
        {
            flags = O_WRONLY | O_CREAT;
            append = 1;
        }
        else
            flags = O_WRONLY | O_TRUNC | O_CREAT;

        char* redirect = prev->args->value;
        *fd = open(redirect, flags, 0644);
        if (*fd < 0)
        {
            write(STDOUT_FILENO, "An error appeared\n", 18);
            free_parsed_part(*parsed);
            free_parse_command(parse_command);
            return NULL;
        }
        else
        {
            if (append == 1)
                lseek(*fd, 0, SEEK_END);
            dup2(*fd, STDOUT_FILENO);
        }
    }

    return parse_command;

}

struct parsed_part *parse_pipes(char *buf)
{
    char *sep = "|";
    struct parsed_part *parsed = parse_all_input(buf, sep);
    return parsed;
}

struct parsed_part *parse_and(char *buf)
{
    char *sep = "&";
    struct parsed_part *parsed = parse_all_input(buf, sep);
    return parsed;
}
