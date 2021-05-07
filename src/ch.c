#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ch.h"

#define SOURCE_EXTENSION ".c"
#define HEADER_EXTENSION ".h"

int buff_index;
char *buff[1000];

void strip_new_line(char *line)
{
    line[strcspn(line, "\n")] = '\0';
}

int file_exists(char *name)
{
    if (access(name, F_OK) == 0)
        return 1;
    return 0;
}

int check_if_function(char *line)
{
    if (line[0] != ' ' && line[strlen(line) - 1] == ')')
        return 1;

    return 0;
}

int write_to_buffer(char *line)
{
    char *new_line = malloc(sizeof(char) * strlen(line));
    strcpy(new_line, line);
    buff[buff_index] = strcat(new_line, ";\n");
    buff_index++;
    return 1;
}

int main(int argc, char *argv[])
{
    FILE *source_file;
    FILE *header_file;
    char line_buff[256];
    char *file_name;
    char *source_file_name;
    char *header_file_name;

    file_name = argv[argc - 1];

    source_file_name = malloc(sizeof(char) * (strlen(file_name) + strlen(SOURCE_EXTENSION)));
    strcpy(source_file_name, file_name);
    strcat(source_file_name, SOURCE_EXTENSION);

    if (!file_exists(source_file_name))
    {
        fprintf(stderr, "source file does not exist\n");
        return 0;
    }

    header_file_name = malloc(sizeof(char) * (strlen(file_name) + strlen(HEADER_EXTENSION)));
    strcpy(header_file_name, file_name);
    strcat(header_file_name, HEADER_EXTENSION);

    source_file = fopen(source_file_name, "a+");

    while (fgets(line_buff, sizeof line_buff, source_file))
    {
        strip_new_line(line_buff);
        if (check_if_function(line_buff) && write_to_buffer(line_buff))
        {
            printf("wrote: %s\n", line_buff);
        }
    }

    fclose(source_file);

    header_file = fopen(header_file_name, "a");
    for (int i = 0; i < buff_index; i++)
    {
        fprintf(header_file, buff[i]);
        free(buff[i]);
    }

    fclose(header_file);

    free(source_file_name);
    free(header_file_name);

    return 0;
}