#include "config.h"

static int get_str_size(char *string);
static void string_array_to_string(char **array, char **out);

//Initialization of the shell
ShellConfig *ShellConfig_init() {
  ShellConfig *retval;

  retval = (ShellConfig *)malloc(sizeof(ShellConfig));

  retval->out_file = "out.txt";
  retval->err_file = "err.txt";

  ShellConfig_set_out_file(retval, DEF_OUT_FILE);
  ShellConfig_set_err_file(retval, DEF_ERR_FILE);
  ShellConfig_set_logger_path(retval, DEF_LOGGER_PATH);
  ShellConfig_set_output_max_size(retval, DEF_MAX_SIZE);
  retval->print_code = 0;

  return retval;
}

void ShellConfig_destroy(ShellConfig *self) {
  free(self);
}

void ShellConfig_set_logger_path(ShellConfig *self, char *logger_path) {
  self->logger_path = logger_path;
}

void ShellConfig_set_out_file(ShellConfig *self, char *out_file) {
  self->out_file = out_file;
  /* if err_file and out_file are equal the Shell should know this.. */
  self->out_err_are_equals = strcmp(self->out_file, self->err_file) == 0;
}

void ShellConfig_set_err_file(ShellConfig *self, char *err_file) {
  self->err_file = err_file;
  /* if err_file and out_file are equal the Shell should know this.. */
  self->out_err_are_equals = strcmp(self->out_file, self->err_file) == 0;
}

void ShellConfig_enable_print_code(ShellConfig *self) {
  self->print_code = 1;
}

void ShellConfig_set_output_max_size(ShellConfig *self, int out_max_size) {
  self->output_max_size = out_max_size;
}

//Get the string to pass to the logger
void ShellConfig_get_logger_call_string(ShellConfig *self, int is_err, int pid, int code, 
                                        char *input, char **command, char **out) {
  char *buffer;
  char *command_str;
  int out_size;

  command_str = (char *)malloc(sizeof(char) * 512);
  buffer = (char *)malloc(sizeof(char) * 512);

  string_array_to_string(command, &command_str);

  //const char *e_option = (is_err) ? "-e" : "";
  //const char *print_code = (self->print_code) ? ("-C -c "+code) : "";
  /*sprintf(buffer, "%s -o %s -m %d -n \"%s\" -s \"%s\" -p %d %s %s\0", 
              self->logger_path, (is_err ? self->err_file : self->out_file),
              self->output_max_size, input, command_str, pid, e_option, print_code);*/

  if(is_err) {
    if (self->print_code) {
      //String with the exit code and error
      sprintf(buffer, "%s -o %s -m %d -n \"%s\" -s \"%s\" -p %d -e -C -c %d\0", 
              self->logger_path, (is_err ? self->err_file : self->out_file),
              self->output_max_size, input, command_str, pid, code);
    }
    else {
      //String without the exit code but with error error
      sprintf(buffer, "%s -o %s -m %d -n \"%s\" -s \"%s\" -p %d -e\0", 
              self->logger_path, (is_err ? self->err_file : self->out_file),
              self->output_max_size, input, command_str, pid);
    }
  }
  else {
    if (self->print_code) {
      //String with the exit code
      sprintf(buffer, "%s -o %s -m %d -n \"%s\" -s \"%s\" -p %d -C -c %d\0", 
              self->logger_path, (is_err ? self->err_file : self->out_file),
              self->output_max_size, input, command_str, pid, code);
    }
    else {
      //String without the exit code
      sprintf(buffer, "%s -o %s -m %d -n \"%s\" -s \"%s\" -p %d\0", 
              self->logger_path, (is_err ? self->err_file : self->out_file),
              self->output_max_size, input, command_str, pid);
    }
  }

  out_size = get_str_size(buffer);

  *out = (char *)malloc(sizeof(char) * out_size+1);

  strncpy(*out, buffer, out_size);

  free(buffer);

  (*out)[out_size] = '\0';
}

//Get the string from the array
static void string_array_to_string(char **array, char **out) {
  int total_size;

  total_size = get_str_size(*array);  
  strncpy(*out, *array, total_size);
  array++;

  while (*array != NULL) {
    total_size += get_str_size(*array);
    strcat(*out, *array);
    array++;
  }

  *out = realloc(*out, sizeof(char) * total_size + 1);

  (*out)[total_size] = '\0';
}

static int get_str_size(char *string) {
  int i;
  for (i = 0; string[i] != EOF && string[i] != '\0' && string[i] != NULL; i++);

  return i;
}
