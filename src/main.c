#include "shell/shell.h"

int main(int argc, char **argv) {
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
