#ifndef __COMMAND__PARSER__H__
#define __COMMAND__PARSER__H__
#define PIPE '|'
#define REDIRECT '>'
#define ASYNC '&'
#define COMMAND_DELIMITER ';'
#define ARG_SEPARATOR ' '
#define LITERAL_SEP '"'

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char ***parse_line(char *line, int * lenght,
                   int *is_async, int *is_pipe, 
                   int *has_redirect, char **redirect_file,
                   char *log_path, int log_path_size);

void free_commands(char ***commands, int length);
#endif
