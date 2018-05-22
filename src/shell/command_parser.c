#include "command_parser.h"

static char ** resize_args(char ** string, int count);
static void add_string_to_args(char ** current_args, int curr_count, 
                               char * line, int start, int count);
static char ** add_command(char * line, int last, int current);
static char **add_log(char *log_path, int log_path_size);
static char **split_ignoring_char(char *command, char delimiter, 
                                  char ignore);

char *** parse_line(char * line, int * length, 
                    int * is_async, int * is_pipe,
                    int * has_redirect, char ** redirect_file,
                    char * log_path, int log_path_size) {
  char curr = '\0';
  int check_enabled = 1;
  int last = 0;
  int retval_index = 0;

  *has_redirect = 0;
  *is_async = 0;
  *is_pipe = 0;

  char *** retval = (char ***)calloc(sizeof(char **), 1024);

  int i;
  for (i = 0; line[i] != EOF && line[i] != '\0'; i++) {
    curr = line[i];

    if (curr == LITERAL_SEP) {
     check_enabled = !check_enabled; 
    }
    else if(check_enabled) {
      if (curr == PIPE) {
        retval[retval_index++]  = add_command(line, last, i);
        retval[retval_index] = add_log(log_path, log_path_size);

        retval_index++;
        last = i + 1;
        *is_pipe = 1;
      } 
      else if (curr == REDIRECT) {
        if ((*has_redirect)) {
          fprintf(stderr, "Error: redirect wrong.\n");
          exit(-1);
        }

        retval[retval_index++] = add_command(line, last, i);
        retval[retval_index] = add_log(log_path, log_path_size);
        
        retval_index++;
        last = i + 1;
        *has_redirect = 1;
      }
      else if (curr == ASYNC) {
        if ((*has_redirect) || (*is_pipe) || 
            (line[i+1] != EOF && line[i+1] != '\0') ) {
          fprintf(stderr, "Error: async wrong");
          exit(-1);
        }
        
        retval[retval_index++] = add_command(line, last, i);
        retval[retval_index] = add_log(log_path, log_path_size);

        retval_index++;
        last = i + 1;
        *is_async = 1;
      }
    }
  }

  if (*has_redirect) {
    char *red = *add_command(line, last, i);
    *redirect_file = red;
  }
  else if (last != i) {
    retval[retval_index++] = add_command(line, last, i);
    retval[retval_index] = add_log(log_path, log_path_size);
  }
  retval = (char ***)realloc(retval, sizeof(char **) * (retval_index+2));
  retval[retval_index+1] = NULL;

  return retval;
}

void free_commands(char *** commands, int length) {
  
  for (int i = 0; i < length; i++)
    free(commands[i]);
  free(commands);
}

static char ** add_command(char * line, int last, int current) {
  char * copy = (char *)malloc(sizeof(char) * (current-last+1));
  char ** retval;
  int count;
  int i = 0;

  /* it is mandatory to trim the string in the left part. */
  while (isspace((unsigned char)(*(line + last + i)))) {
    i++;
  }
  strncpy(copy, line+last+i, current - last); 
  copy[current-last] = '\0';

  retval = split_ignoring_char(copy, ARG_SEPARATOR, LITERAL_SEP);
  
  free(copy);
  return retval;
}

static char **add_log(char *log_path, int log_path_size) {
  char **retval;
  int count;

  retval = split_ignoring_char(log_path, ARG_SEPARATOR, LITERAL_SEP);
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

static char **split_ignoring_char(char *command, char delimiter, 
                                  char ignore) {
  char **retval = (char **)malloc(sizeof(char *)*strlen(command));
  char curr;
  int last = 0;
  int check_enabled = 1;
  int index = 0;
  int i;
  for (i = 0; command[i] != EOF && command[i] != '\0'; i++) {
    curr = command[i];

    if (curr == ignore) {
     check_enabled = !check_enabled; 
    }
    
    if (check_enabled && curr == delimiter) {
      retval[index] = (char *)malloc(sizeof(char) * (i - last + 1)); 
      strncpy(retval[index], command + last, i - last); 
      retval[index][i-last] = '\0';
      index++;
      last = i + 1;
    }
  }

  if (last < i) {
    char *copy = (char *)malloc(sizeof(char) * (i - last + 1));
    strncpy(copy, command + last, i - last);
    copy[i-last] = '\0';
    retval[index] = copy;
    index++;
    last = i;
  }

  retval = (char **)realloc(retval, sizeof(char *) * (index+1));
  retval[index] = NULL;
  return retval;
}
