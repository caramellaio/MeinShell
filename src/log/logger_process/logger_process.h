#ifndef __LOGGER__PROCESS__H__
#define __LOGGER__PROCESS__H__
#define LOGGER(x) ((Logger *)x)
#define LOGGER_ALLOC(x) ((Logger *)malloc(sizeof(Logger *) * x))
#define BUFFER_SIZE 80
#define STRING(x) (char *)x

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

typedef struct Logger {
  char *process;
  char *sub_process;
  char *out_file;
  char *err_file;
  int code;
  int print_code;
  int pid;
  int cmd_size;
} Logger;

Logger *Logger_init();
void Logger_set_code(Logger *self, int code);
void Logger_enable_print_code(Logger *self);
void Logger_set_pid(Logger *self, int pid);
void Logger_set_out_file(Logger *self, char *outfile);
void Logger_set_err_file(Logger *self, char *errfile);
void Logger_set_cmd_size(Logger *self, int value);
void Logger_set_process(Logger *self, char *process);
void Logger_set_sub_process(Logger *self, char *subprocess);
void Logger_destroy(Logger *self);

void Logger_process_run();
void Logger_log(Logger *self);
#endif
