#define ARG_COUNT 5
#include "arguments_handling/arg.h"
#include "arguments_handling/arg_handler.h"
#include "log/log.h"

void setup_setting(int argc, char ** argv);
// TODO: should main capture sigterm in order to kill child process?
ArgsHandler * args_handler;
Logger * logger;

int main(int argc, char ** argv) {
  setup_setting(argc, argv);
  return 0;
}

void setup_setting(int argc, char ** argv) {
  args_handler = ArgsHandler_init(ARG_COUNT);
  logger = Logger_init();
  ArgsHandler_insert_arguments(args_handler, Arg_init("-h", "--help", NULL, args_handler, 0, ArgsHandler_print)); 
  ArgsHandler_insert_arguments(args_handler, Arg_init("-o", "--out-file", NULL, logger, 1, Logger_set_out_file)); 
  ArgsHandler_insert_arguments(args_handler, Arg_init("-e", "--err-file", NULL, logger, 1, Logger_set_err_file)); 
  ArgsHandler_insert_arguments(args_handler, Arg_init("-c", "--code", NULL, logger, 0, Logger_enable_print_code)); 
  ArgsHandler_insert_arguments(args_handler, Arg_init("-m", "--max-size", NULL, logger, 1, Logger_set_cmd_size)); 

  if (! ArgsHandler_handle_input(args_handler, argc, argv)) {
    shell_exit(-1, "Invalid argument passing..."); 
  }
}

// this is not a permanent function...
void shell_exit(int code, char * message) {
  printf("%s\n",message);
  exit(code);
}

// glue code idea: function that takes a void * (which is the function...
