#ifndef __LOG__H__
#define __LOG_H__

#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 80
typedef struct Logger {
  char * out_file;
  char * err_file;
  int print_code;
  int cmd_size;
  int id_count; // TODO: add to logger logic.
} Logger;

void Logger_set_out_file(Logger * self, char * value);
void Logger_set_err_file(Logger * self, char * value);
void Logger_enable_print_code(Logger * self);
void Logger_set_cmd_size(Logger * self, int value);

Logger * Logger_init();
Logger * Logger_create(char * out_file, char *err_file, int print_code,int cmd_size);
Logger * Logger_destroy(Logger * self);
int Logger_log(Logger * self, char * process, int pid, int err_fd, int out_fd, int code);
int Logger_clean(Logger * self);
#endif
