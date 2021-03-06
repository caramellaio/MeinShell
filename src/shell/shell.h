#ifndef __SHELL__H__
#define __SHELL__H__

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define SHELL(x) (Shell *)x
#define SHELL_ALLOC(x) SHELL(malloc(sizeof(Shell) * x))
#define NO_PROCESS -1
#define STRING(x) (char *)x

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <signal.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../arguments_handling/arg_handler.h"
#include "config.h"
#include "command_parser.h"

typedef struct Shell {
  // TODO: change it into a linked list of process

  /* changes on runtime */
  int running_process_pids;
  char *executing_line;
  ShellConfig *config;
  /* given at the beginning */
} Shell;

Shell *Shell_init();
void Shell_destroy();

void Shell_start(Shell *self);
void Shell_print(Shell *self);
void Shell_print_error(Shell *self, char *error, int do_exit);
void Shell_main_loop(Shell *self);
void Shell_configure(Shell *self, int argc, char **argv);
void Shell_cd(Shell *self, char *new_dir);
void Shell_print_help(Shell *self);
void Shell_kill_running_process(Shell *self);
void Shell_on_quit_request(Shell *self, int signo);
void Shell_get_logger_str(Shell *self, int is_err, int pid, int  code, 
                          char **cmd_args, char ***out);
void Shell_set_running_process(Shell *self, int pid);
void Shell_exit(Shell *self, int code);
#endif
