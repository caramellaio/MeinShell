#include "logger_process.h"

static void get_input_opts(int argc, char ** argv, Logger * logger);

static void log_header(Logger * self, FILE * file);
static void log_output(Logger * self, FILE * file);
static void log_code(Logger * self, FILE * file);
static void log_end(Logger * self, FILE * file);


static void set_code(void * logger_void, char * code_str);
static void set_pid(void * logger_void, char * pid_str);
static void enable_print_code(void * self, char * foo);
static void get_input_opts(int argc, char ** argv, Logger * logger);

void Logger_process_run(int argc, char ** argv) {
  Logger * logger = Logger_init();  
  get_input_opts(argc, argv, logger);
  Logger_log(logger);
  Logger_destroy(logger);
}

void Logger_log(Logger * self) {
  FILE * out = fopen(self->out_file, "a+");
  
  log_header(self, out);
  log_output(self, out);
  if (self->print_code) {
    log_code(self, out);
  }
  log_end(self, out);
  fclose(out);
}

static void log_header(Logger * self, FILE * file) {
  time_t now = time(NULL);

  char buffer[100];
  strftime(buffer, 100, "%Y-%m-%d %H:%M:%S", localtime(&now));
  //fprintf(file, "SHELL ID: %d\n", self->id_count);
  fprintf(file, "PID: %d\n", self->pid);
  fprintf(file, "COMMAND: %s\n", self->process);
  fprintf(file, "SUB COMMAND: %s\n", self->sub_process); // not mandatory
  fprintf(file, "DATE: %s\n", buffer);
  fprintf(file, "OUTPUT:\n");
}

static void log_output(Logger * self, FILE * file) {
  int value = 1;
  char buffer[BUFFER_SIZE];

  while((value = read(fileno(stdin), buffer, BUFFER_SIZE)) != 0) {
    if (value < 0) {
      perror("Error: unable to read from stdin.");
      exit(-1);
    }
    printf("%s", buffer);
    fprintf(file,"%s", buffer);  
  }
}

static void log_code(Logger * self, FILE * file) {
  fprintf(file, "CODE: %d\n", self->code);
}
static void log_end(Logger * self, FILE * file) {
  fprintf(file, "-------------------------------\n");
}

Logger * Logger_init() {
  Logger * retval = LOGGER_ALLOC(1);  
  return retval;
}

void Logger_set_code(Logger * self, int code) {
  self->code = code;
}

void Logger_enable_print_code(Logger * self) {
  self->print_code = 1;
}

void Logger_set_pid(Logger * self, int pid) {
  self->pid = pid;
}

void Logger_set_process(Logger * self, char * process) {
  self->process = process;
}

void Logger_set_sub_process(Logger * self, char * subprocess) {
  self->sub_process = subprocess;
}

void Logger_set_out_file(Logger * self, char * outfile) {
  self->out_file = outfile;
}

void Logger_destroy(Logger * self) {
  free(self);
}

static void set_pid(void * logger_void, char * pid_str) {
  Logger * logger  = LOGGER(logger_void);
  int pid = atoi(pid_str);
  if (0 == pid) {
    fprintf(stderr, "Error: invalid argument casting \"%s\" in set_pid", pid_str);
    exit(-1);
  }
  Logger_set_pid(logger, pid);
}

static void set_code(void * logger_void, char * code_str) {
  Logger * logger  = LOGGER(logger_void);
  int code = atoi(code_str);
  Logger_set_code(logger, code);
}

static void set_out_file(void * logger_void, char *out_file) {
  Logger * logger = LOGGER(logger_void);
  Logger_set_out_file(logger, out_file);
} 

static void enable_print_code(void * logger_void, char * foo) {
  Logger * logger  = LOGGER(logger_void);
  Logger_enable_print_code(logger);
}

static void get_input_opts(int argc, char ** argv, Logger * logger) {
  const int ARG_COUNT = 5;
  ArgsHandler * args_handler = ArgsHandler_init(ARG_COUNT);

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
  
  ArgsHandler_insert_arguments(args_handler, Arg_init("-c", "--code",
                                                      STRING(NULL),
                                                      logger, 1,
                                                      set_code));

  ArgsHandler_insert_arguments(args_handler, Arg_init("-C", "--print-code",
                                                      STRING(NULL),
                                                      logger, 0,
                                                      enable_print_code));

  if (! ArgsHandler_handle_input(args_handler, argc, argv)) {
    perror("Error: unable to parse arguments...");
    exit(-1);
  }

  ArgsHandler_destroy(args_handler);
}
