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

  Shell * shell = Shell_init();
  Shell_configure(shell, argc, argv);
  Shell_start(shell);
  Shell_destroy(shell);
  //Execute_commands_with_pipe(&elem, 3);
  return 0;
}
