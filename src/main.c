#include "arguments_handling/arg.h"
#include "arguments_handling/arg_handler.h"
#include "log/log.h"
#include "command_processor/command_processor.h"

#define ARG_COUNT 5
#define STRING(x) (char *)x
void setup_setting(int argc, char ** argv);
void shell_exit(int code, char * message);
// TODO: should main capture sigterm in order to kill child process?
ArgsHandler * args_handler;
Logger * logger;

int main(int argc, char ** argv) {
  setup_setting(argc, argv);
  char *ls[] = {"ls", NULL};
  char *log[] = {"log", NULL};
  char *log2[] = {"log", NULL};
  char *log3[] = {"log", NULL};
  char *grep[] = {"grep", "bin", NULL};
  char *wc[] = {"wc", NULL};
  char **cmd[] = {ls, log, grep, log2, wc, log3, NULL};
  loop_pipe(cmd, 1, "redir.txt");
  //Execute_commands_with_pipe(&elem, 3);
  return 0;
}

void setup_setting(int argc, char ** argv) {
  args_handler = ArgsHandler_init(ARG_COUNT);
  logger = Logger_init();
  /* print help and close the program */
  ArgsHandler_insert_arguments(args_handler, Arg_init("-h", "--help", STRING(NULL), args_handler, 0, ArgsHandler_print)); 
  /* set the Logger output file */
  ArgsHandler_insert_arguments(args_handler, Arg_init("-o", "--out-file", STRING(NULL), logger, 1, Logger_set_out_file)); 
  /* set the Logger error file */
  ArgsHandler_insert_arguments(args_handler, Arg_init("-e", "--err-file", STRING(NULL), logger, 1, Logger_set_err_file)); 
  /* set if the Logger have to print the return code of the processes*/
  ArgsHandler_insert_arguments(args_handler, Arg_init("-c", "--code", STRING(NULL), logger, 0, Logger_enable_print_code)); 
  /* set the max size in characters of the output of every process */
  ArgsHandler_insert_arguments(args_handler, Arg_init("-m", "--max-size", STRING(NULL), logger, 1, Logger_set_cmd_size)); 
  /* set the logger process executable path 
    COMMENTED because not yet implemented.
  */
  //ArgsHandler_insert_arguments(args_handler, Arg_init("-l", "--logger-path", STRING(NULL), shell, 1, Shell_set_logger_path)); 

  if (! ArgsHandler_handle_input(args_handler, argc, argv)) {
    shell_exit(-1, "Invalid argument passing..."); 
  }
  
  ArgsHandler_destroy(args_handler);
}

// this is not a permanent function...
void shell_exit(int code, char * message) {
  printf("%s\n",message);
  exit(code);
}
