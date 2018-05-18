#ifndef __SHELL__CONFIG__H__
#define __SHELL__CONFIG__H__

#define DEF_OUT_FILE "out.txt"
#define DEF_ERR_FILE "err.txt"
#define DEF_MAX_SIZE 500
#define DEF_LOGGER_PATH "/usr/bin/log"

#include <stdlib.h>

typedef struct ShellConfig {
  char * logger_path;
  char * out_file;
  char * err_file;
  int print_code;
  int output_max_size;
  int out_err_are_equals;
} ShellConfig;

ShellConfig * ShellConfig_init();

void ShellConfig_destroy(ShellConfig * self);


void ShellConfig_set_logger_path(ShellConfig * self, 
                                 char * logger_path);

void ShellConfig_set_outfile(ShellConfig * self, 
                             char * out_file);

void ShellConfig_set_err_file(ShellConfig * self, 
                              char * err_file);

void ShellConfig_enable_print_code(ShellConfig * self);

void ShellConfig_set_output_max_size(ShellConfig * self, 
                                     int out_max_size);
#endif
