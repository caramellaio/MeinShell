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
  int c;
  char ** values = Read_command("pippo | pluto", "|", &c);
  char * elem[] = {"ls", "wc", "grep \"9\""};
  Execute_commands_with_pipe(&elem, 3);
  return 0;
}

void setup_setting(int argc, char ** argv) {
  args_handler = ArgsHandler_init(ARG_COUNT);
  logger = Logger_init();
  ArgsHandler_insert_arguments(args_handler, Arg_init("-h", "--help", STRING(NULL), args_handler, 0, ArgsHandler_print)); 
  ArgsHandler_insert_arguments(args_handler, Arg_init("-o", "--out-file", STRING(NULL), logger, 1, Logger_set_out_file)); 
  ArgsHandler_insert_arguments(args_handler, Arg_init("-e", "--err-file", STRING(NULL), logger, 1, Logger_set_err_file)); 
  ArgsHandler_insert_arguments(args_handler, Arg_init("-c", "--code", STRING(NULL), logger, 0, Logger_enable_print_code)); 
  ArgsHandler_insert_arguments(args_handler, Arg_init("-m", "--max-size", STRING(NULL), logger, 1, Logger_set_cmd_size)); 

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
