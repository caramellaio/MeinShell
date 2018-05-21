#include "logger_process.h"
#include "../../arguments_handling/arg_handler.h"
#include "../../arguments_handling/arg.h"

void get_input_opts(int argc, char **argv, Logger *logger);

void set_pid(void *logger_void, char *pid_str) {
  Logger *logger = LOGGER(logger_void);
  int pid = atoi(pid_str);
  if (0 == pid) {
    fprintf(stderr, "Error: invalid argument casting \"%s\" in set_pid", pid_str);
    exit(-1);
  }
  Logger_set_pid(logger, pid);
}

void set_code(void *logger_void, char *code_str) {
  Logger *logger = LOGGER(logger_void);
  int code = atoi(code_str);
  Logger_set_code(logger, code);
}

void set_out_file(void *logger_void, char *out_file) {
  Logger *logger = LOGGER(logger_void);
  Logger_set_out_file(logger, out_file);
}

void set_err_true(void *logger_void) {
  Logger *logger = LOGGER(logger_void);
  Logger_set_err_true(logger);
}

void set_output_max_size(void *logger_void, char *max_size){
  Logger *logger = LOGGER(logger_void);
  int output_max_size = atoi(max_size);
  Logger_set_cmd_size(logger, output_max_size);
}

void enable_print_code(void *logger_void, char *foo) {
  Logger *logger = LOGGER(logger_void);
  Logger_enable_print_code(logger);
}

int main(int argc, char **argv) {
  Logger *logger = Logger_init();
  get_input_opts(argc, argv, logger);
  Logger_process_run(logger);
  return 0;
}

void get_input_opts(int argc, char **argv, Logger *logger) {
  const int ARG_COUNT = 8;
  ArgsHandler *args_handler = ArgsHandler_init(ARG_COUNT);

  ArgsHandler_insert_arguments(args_handler, Arg_init("-h", "--help",
                                                      STRING(NULL),
                                                      args_handler, 0,
                                                      ArgsHandler_print));

  ArgsHandler_insert_arguments(args_handler, Arg_init("-o", "--out-file",
                                                      STRING(NULL),
                                                      logger, 1,
                                                      set_out_file));

  ArgsHandler_insert_arguments(args_handler, Arg_init("-n", "--process-name",
                                                      STRING(NULL),
                                                      logger, 1,
                                                      Logger_set_process));

  ArgsHandler_insert_arguments(args_handler, Arg_init("-p", "--pid",
                                                      STRING(NULL),
                                                      logger, 1,
                                                      set_pid));
  
  ArgsHandler_insert_arguments(args_handler, Arg_init("-e", "--is-err", 
                                                      STRING(NULL), 
                                                      logger, 0, 
                                                      set_err_true)); 

  ArgsHandler_insert_arguments(args_handler, Arg_init("-m", "--max-size", STRING(NULL), 
                                                      logger, 1, 
                                                      set_output_max_size));

  ArgsHandler_insert_arguments(args_handler, Arg_init("-c", "--code",
                                                      STRING(NULL),
                                                      logger, 1,
                                                      set_code));

  ArgsHandler_insert_arguments(args_handler, Arg_init("-C", "--print-code",
                                                      STRING(NULL),
                                                      logger, 0,
                                                      enable_print_code));

  if (!ArgsHandler_handle_input(args_handler, argc, argv)) {
    perror("Error: unable to parse arguments...");
    exit(-1);
  }

  //ArgsHandler_destroy(args_handler);
}
