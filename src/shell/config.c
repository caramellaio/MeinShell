#include "config.h"

ShellConfig * ShellConfig_init() {
  ShellConfig * retval;

  retval = (ShellConfig *)malloc(sizeof(ShellConfig));

  ShellConfig_set_out_file(retval, DEF_OUT_FILE);
  ShellConfig_set_err_file(retval, DEF_ERR_FILE);
  ShellConfig_set_logger_path(retval, DEF_LOGGER_PATH);
  ShellConfig_set_output_max_size(retval, DEF_MAX_SIZE);
  retval->print_code = 0;

  return retval;
}

void ShellConfig_destroy(ShellConfig * self) {
  free(self);
}


void ShellConfig_set_logger_path(ShellConfig * self, 
                                 char * logger_path) {
  self->logger_path = logger_path;
}

void ShellConfig_set_out_file(ShellConfig * self, 
                              char * out_file) {
  self->out_file = out_file;
  /* if err_file and out_file are equal the Shell should know this.. */
  self->out_err_are_equals = strcmp(self->out_file, self->err_file) == 0;
}

void ShellConfig_set_err_file(ShellConfig * self, 
                              char * err_file) {
  self->err_file = err_file;
  /* if err_file and out_file are equal the Shell should know this.. */
  self->out_err_are_equals = strcmp(self->out_file, self->err_file) == 0;
}

void ShellConfig_enable_print_code(ShellConfig * self) {
  self->print_code = 1;
}

void ShellConfig_set_output_max_size(ShellConfig * self, 
                                     int out_max_size) {
  self->output_max_size = out_max_size;
}
