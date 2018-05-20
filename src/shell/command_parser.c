/*char ***parse_line(char *line, int * lenght,
                  int *is_async, int *is_pipe, 
                  int * has_redirect, char ** redirect_file) {
  char ** retval;
  
  int count;
  retval = Read_command(line, REDIRECT, &count);

  if (redir_c > 2) {
    fprintf(stderr, "%sError: wrong number of redirect operators.%s.\n", ANSI_COLOR_RED, ANSI_COLOR_RESTORE);
    return NULL;
  }

  if (redir_c == 2) {
    *redirect_file = retval[1];
    char * tmp = retval[0];
    free(retval);
    retval = parse_line(retval[0], length, is_async, 
                        is_pipe, has_redirect, NULL);
    *has_redirect = 1;
  }
  else {
    has_redirect = 0;
    free(retval);
  }

  retval = Read_command(line, ASYNC, &count);
  if (count == 2) {
    char * tmp = retval[0];
    free(retval);
    retval = parse_line(tmp, length, is_async,
                        is_pipe, has_redirect, NULL);
  }
  
  free(retval);
  retval = Read_command(line, PIPE, &count);
  free(retval);
}*/

#include "command_parser.h"

static char ** resize_args(char ** string, int count);
static void add_string_to_args(char ** current_args, int curr_count, char * line,
                               int start, int count);
static char ** Read_cmd(char * command, char * delimiter, int * count);
static char ** add_command(char * line, int last, int current);

char *** parse_line(char * line, int * length, 
                    int * is_async, int * is_pipe,
                    int * has_redirect, char ** redirect_file) {
  char curr = '\0';
  int check_enabled = 1;
  int last = 0;
  int retval_index = 0;

  char *** retval = (char ***)malloc(sizeof(char **) * 1024);

  int i;
  for (i = 0; line[i] != EOF && line[i] != '\0'; i++) {
    curr = line[i];

    if (curr == LITERAL_SEP) {
     check_enabled = !check_enabled; 
    }
    else if(check_enabled) {
      if (curr == PIPE) {
        retval[retval_index]  = add_command(line, last, i);

        last = i + 1;
        retval_index++;
        *is_pipe = 1;
      } 
      else if (curr == REDIRECT) {
        if ((*has_redirect)) {
          fprintf(stderr, "Error: redirect wrong.\n");
          exit(-1);
        }

        retval[retval_index] = add_command(line, last, i);
        last = i + 1;
        retval_index++;
        *has_redirect = 1;
      }
      else if (curr == ASYNC) {
        if ((*has_redirect) || (*is_pipe) || 
            (line[i+1] != EOF && line[i+1] != '\0') ) {
          fprintf(stderr, "Error: async wrong");
          exit(-1);
        }
        
        retval[retval_index] = add_command(line, last, i);
        retval_index++;
        last = i + 1;
        *is_async = 1;
      }
    }
  }

  if (*has_redirect) {
    char * red = add_command(line, last, i);
    *redirect_file = red;
  }
  else if (last != i) {
    retval[retval_index] = add_command(line, last, i);
  }
  retval = (char ***)realloc(retval, sizeof(char **) * (retval_index+2));
  retval[retval_index+1] = NULL;

  return retval;
}
// TODO: free line strings...

//char ** string_list_copy(char ** current_args, 

void free_commands(char *** commands, int length) {
  
  for (int i = 0; i < length; i++)
    free(commands[i]);
  free(commands);
}

static char ** add_command(char * line, int last, int current) {
  char * copy = (char *)malloc(sizeof(char) * (current-last+1));
  char ** retval;
  int count;
  strncpy(copy, line+last, current - last); 
  copy[current-last] = '\0';

  retval = Read_cmd(copy, " ", &count);
  
  free(copy);
  return retval;
}

static void add_string_to_args(char ** current_args, int curr_count, char * line, int start, int count) {
  current_args[curr_count] = (char *)malloc(sizeof(char) * count);
  strncpy(current_args[curr_count], line+start, count - 1); // last char should not be copied.
  current_args[curr_count][count-1] = '\0';
}

static char ** resize_args(char ** string, int count) {
  char ** tmp;

  tmp = (char **)realloc(string, count +1);
  tmp[count] = NULL;

  return tmp;
}

static char ** Read_cmd(char * command, char * delimiter, int * count) {
  char * token;
  char * cmd;
  char ** retval = (char **)malloc(sizeof(char *)*strlen(command));
  int i = 1;
  
  cmd = strdup(command);
  token = strtok(cmd, delimiter);
  retval[0] = token;

  while (token = strtok(0, delimiter)) {  
    retval[i] = token;
    i++;
  }

  retval = realloc(retval, sizeof(char *) * (i+1));
  retval[i] = NULL;
  *count = i; 
  return retval;
}
