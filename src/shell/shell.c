#include "shell.h"

static int is_absolute_path(char * path);
static char * get_command_line(Shell * self, size_t *len);

Shell * Shell_init() {
  Shell * retval; 

  retval = SHELL_ALLOC(1);
  retval->running_process_pids = NO_PROCESS;
  retval->config = ShellConfig_init();
}
void Shell_destroy(Shell * self) {
  ShellConfig_destroy(self->config);
  free(self);
  self = NULL;
}
int Shell_run_command(Shell * self, char * command);

void Shell_start(Shell * self) {
  Shell_main_loop(self);
}
void Shell_print(Shell * self) {
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  printf("%s> ", cwd);
  fflush(stdin);
}
void Shell_print_error(Shell * self, char * error, int do_exit) {
  printf(stderr, "%s%s\n",ANSI_COLOR_RED);
  fflush(stdin);
  if (do_exit) {
    Shell_exit(self, EXIT_FAILURE);
  }
}
void Shell_main_loop(Shell * self) {
  char buffer[100];
  char *input;
  for (;;) {
    //Shell_print(self);
    snprintf(buffer, sizeof(buffer), "%s:%s $ ", getenv("USER"), getcwd(NULL, 1024));
    input = readline(buffer);
    if (!input)
      break;
    free(input);
  }
}

void Shell_configure(Shell * self, int argc, char * argv) {
  int result;
  
  ArgsHandler *args_handler = ArgsHandler_init(6);
  /* print help and close the program */
  ArgsHandler_insert_arguments(args_handler, Arg_init("-h", "--help", STRING(NULL), 
                               args_handler, 0, ArgsHandler_print)); 
  /* set the Logger output file */
  ArgsHandler_insert_arguments(args_handler, Arg_init("-o", "--out-file", STRING(NULL), 
                               self->config, 1, ShellConfig_set_out_file)); 
  /* set the Logger error file */
  ArgsHandler_insert_arguments(args_handler, Arg_init("-e", "--err-file", STRING(NULL), 
                               self->config, 1, ShellConfig_set_err_file)); 
  /* set if the Logger have to print the return code of the processes*/
  ArgsHandler_insert_arguments(args_handler, Arg_init("-c", "--code", STRING(NULL), 
                               self->config, 0, ShellConfig_enable_print_code)); 
  /* set the max size in characters of the output of every process */
  ArgsHandler_insert_arguments(args_handler, Arg_init("-m", "--max-size", STRING(NULL), 
                               self->config, 1, ShellConfig_set_output_max_size)); 
  /* set the logger process executable path 
  */
  ArgsHandler_insert_arguments(args_handler, Arg_init("-l", "--logger-path", STRING(NULL), 
                               self->config, 1, ShellConfig_set_logger_path)); 

  result = ArgsHandler_handle_input(args_handler, argc, argv);
  
  ArgsHandler_destroy(args_handler);
  
  if (result == -1) {
    Shell_print_error(self, "Invalid argument passing...", 1); 
  }

 // not yet implemented 
}
void Shell_cd(Shell * self, char * new_dir) {
  if (! is_absolute_path(new_dir)) {
    // TODO: improve to handle ../.. 
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    new_dir = strcat(cwd, new_dir);
  }

 int ret = chdir(new_dir);

 if (ret < 0) {
  Shell_print_error(self, "Error: cd failed", 0);
 }
}

void Shell_print_help(Shell * self) {
  printf("print command has not been implemented yet.\n");
}
void Shell_kill_running_process(Shell * self) {
  if (self->running_process_pids != NO_PROCESS)
    kill(self->running_process_pids, SIGKILL);
}
void Shell_exit(Shell * self, int code) {
  Shell_kill_running_process(self);
  printf("Shell exit with code: %d", code);
}

static int is_absolute_path(char * path) {
  return 1;
}
