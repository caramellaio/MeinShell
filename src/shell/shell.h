#ifndef __SHELL__H__
#define __SHELL__H__
#include <stdio.h>
#include <stdlib.h>
#include "config.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define SHELL(x) (Shell*)x
#define SHELL_ALLOC(x) SHELL(malloc(sizeof(Shell)*x))

typedef struct Shell {
  // TODO: change it into a linked list of process

  /* changes on runtime */
  int running_process_pids;
  char * current_path;

  ShellConfig * config;
  /* given at the beginning */
} Shell;

int Shell_run_command(Shell * self, char * command);
void Shell_print(Shell * self);
void Shell_print_error(Shell * self, char * error, int do_exit);
void Shell_main_loop(Shell * self);
void Shell_configure(Shell * self, int argc, char * argv);
void Shell_cd(Shell * self, char * new_dir);
void Shell_print_help(Shell * self);
void Shell_kill_running_process(Shell * self);
void Shell_exit(Shell * self, int code);
#endif
