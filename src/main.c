#include "shell/shell.h"
#include "signal.h"

static void catch_function(int signo) {
  puts("\nKilling process\n");
}

int main(int argc, char **argv) {
  if (signal(SIGINT, catch_function) == SIG_ERR) {
      fputs("An error occurred while setting a signal handler.\n", stderr);
      return 1;
  }

  char *ls[] = {"ls", NULL};
  char *log[] = {"log", NULL};
  char *log2[] = {"log", NULL};
  char *log3[] = {"log", NULL};
  char *grep[] = {"grep", "bin", NULL};
  char *wc[] = {"wc", NULL};
  char **cmd[] = {ls, log, grep, log2, wc, log3, NULL};
  loop_pipe(cmd, 1, "redir.txt");
  Shell * shell = Shell_init();
  Shell_configure(shell, argc, argv);
  Shell_start(shell);
  Shell_destroy(shell);
  //Execute_commands_with_pipe(&elem, 3);
  return 0;
}