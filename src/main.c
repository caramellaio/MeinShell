#include "shell/shell.h"
#include "signal.h"

#define CYAN "\x1b[36m"
#define YELLOW "\x1b[33m"
#define RESET "\x1b[0m"

Shell *shell;

//Catch the interrupt signal
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

  //Print presentation
  printf(CYAN "\nINTERACTIVE SHELL:\n" RESET "\t- For exit use " YELLOW "\"quit\"" RESET " command\n\n");


  shell = Shell_init();
  Shell_configure(shell, argc, argv);
  Shell_start(shell);
  Shell_destroy(shell);
  return 0;
}
