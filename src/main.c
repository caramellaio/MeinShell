#include "shell/shell.h"
#include "signal.h"

Shell *shell;

static void catch_function(int signo) {
  if (NULL == shell)
    exit(0);

  Shell_on_quit_request(shell, signo);
}

int main(int argc, char **argv) {
  if (signal(SIGINT, catch_function) == SIG_ERR) {
      fputs("An error occurred while setting a signal handler.\n", stderr);
      return 1;
  }

  shell = Shell_init();
  Shell_configure(shell, argc, argv);
  Shell_start(shell);
  Shell_destroy(shell);
  return 0;
}
