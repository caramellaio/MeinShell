#include "config.h"

static int get_str_size(char *string);
static void string_array_to_string(char **array, char **out);

ShellConfig *ShellConfig_init() {
  ShellConfig *retval;

  retval = (ShellConfig *)malloc(sizeof(ShellConfig));

  retval->out_file = "";
  retval->err_file = "";

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

char *ShellConfig_get_logger_call_string(ShellConfig *self, int is_err, int pid,
                                         int code, char * input, char **command) {
  char buffer[512];
  char command_str[512];
  char *retval;
  int retval_size;

  if (self->print_code) {
    sprintf(buffer, "%s -o %s -m %d -n %s -s %s -p %d -C -c %d\0", 
            self->logger_path, is_err ? self->err_file : self->out_file,
            self->output_max_size, input, command_str, pid, code);
  }
  else {
    sprintf(buffer, "%s -o %s -m %d -n %s -s %s -p %d\0", 
            self->logger_path, is_err ? self->err_file : self->out_file,
            self->output_max_size, input, command_str, pid);
  }

  retval_size = get_str_size(buffer);

  retval = (char *)malloc(sizeof(char)*retval_size+1);

  strncpy(retval, buffer, retval_size);
  retval[retval_size] = '\0';
  
  return retval;
}

static void string_array_to_string(char **array, char **out) {
  while (*array != NULL) {
    strcat(*out, *array);
    array++;
  }
}
static int get_str_size(char *string) {
  int i;
  for (i = 0; string[i] != EOF && string[i] != '\0' && string[i] != NULL; i++);

  return i;
}
