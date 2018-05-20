#include "config.h"

static int get_str_size(char *string);

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

char *ShellConfig_get_logger_call_string(ShellConfig *self, int is_err) {
  char *logger_str;
  char max_size_str[256];

  int logger_p_size;
  int out_file_size; 
  int err_file_size;
  int max_size_size;

  int logger_str_size; 

  logger_p_size = get_str_size(self->logger_path);
  if (is_err) {
    out_file_size = get_str_size(self->err_file);
  }
  else {
    out_file_size = get_str_size(self->out_file);
  }

  sprintf(max_size_str, "%d\0", self->output_max_size);

  max_size_size = get_str_size(max_size_str);

  logger_str_size = logger_p_size + out_file_size + max_size_size + 12;

  if (self->print_code)
    logger_str_size += 4;

  logger_str = (char *)malloc(sizeof(char)*logger_str_size+1);

  strcpy(self->logger_path, logger_str);
  strcat(logger_str, " -o ");
  strcat(logger_str, self->out_file);
  strcat(logger_str, " -m ");
  strcat(logger_str, max_size_str);

  if (self->print_code) {
    strcat(logger_str, " -p ");
  }

  logger_str[logger_str_size] = '\0';

  return logger_str;
}

static int get_str_size(char *string) {
  int i;
  for (i = 0; string[i] != EOF && string[i] != '\0' && string[i] != NULL; i++);

  return i;
}
