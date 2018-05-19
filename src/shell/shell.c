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
  rl_bind_key('\t', rl_complete);
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
  kill(self->running_process_pids, SIGKILL);
}
void Shell_exit(Shell * self, int code) {
  Shell_kill_running_process(self);
  printf("Shell exit with code: %d", code);
}

static int is_absolute_path(char * path) {
  return 1;
}
