#include "logger_process.h"

static void get_input_opts(int argc, char **argv, Logger *logger);

static void log_header(Logger *self, FILE *file);
static void log_output(Logger *self, FILE *file);
static void log_code(Logger *self, FILE *file);
static void log_end(Logger *self, FILE *file);
//static void log_header(Logger *self, int file);
//static void log_output(Logger *self, int file);
//static void log_code(Logger *self, int file);
//static void log_end(Logger *self, int file);

void Logger_process_run(Logger *logger) {
  Logger_log(logger);
  Logger_destroy(logger);
}

void Logger_log(Logger *self) {
  FILE *out = fopen(self->out_file, "a+");

  //int out = open (self->out_file, O_CREAT, 0777);

  log_header(self, out);
  log_output(self, out);
  if (self->print_code) {
    log_code(self, out);
  }
  log_end(self, out);
  fclose(out);
  //close(out);
}

static void log_header(Logger *self, FILE *file) {
  //static void log_header(Logger *self, int file) {
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

static void log_output(Logger *self, FILE *file) {
  //static void log_output(Logger *self, int file) {
  int value = 1;
  char buffer[BUFFER_SIZE];

  /*while ((value = read(fileno(stdin), buffer, BUFFER_SIZE)) != 0) {
    if (value < 0) {
      perror("Error: unable to read from stdin.");
      exit(-1);
    }
    printf("%s", buffer);
    fprintf(file, "%s", buffer);
  }*/
  while ( (value = read(fileno(stdin), buffer, BUFFER_SIZE)) != 0 && (value < self->cmd_size)){
    if (value < 0) {
      perror("Error: unable to read from stdin.");
      exit(-1);
    }
    char *str = (char*)malloc(value+1);
    strncpy(str, buffer, value);
    str[value+1]='\0';  
    write(fileno(stdout),str,value+1);
    fprintf(file, "%s",str);
    free(str);
  }
}

static void log_code(Logger *self, FILE *file) {
//static void log_code(Logger *self, int file) {
  fprintf(file, "CODE: %d\n", self->code);
}
static void log_end(Logger *self, FILE *file) {
//static void log_end(Logger *self, int file) {
  fprintf(file, "-------------------------------\n");
}

Logger *Logger_init() {
  Logger *retval = LOGGER_ALLOC(1);
  return retval;
}

void Logger_set_code(Logger *self, int code) {
  self->code = code;
}

void Logger_enable_print_code(Logger *self) {
  self->print_code = 1;
}

void Logger_set_pid(Logger *self, int pid) {
  self->pid = pid;
}

void Logger_set_process(Logger *self, char *process) {
  self->process = process;
}

void Logger_set_sub_process(Logger *self, char *subprocess) {
  self->sub_process = subprocess;
}

void Logger_set_out_file(Logger *self, char *outfile) {
  self->out_file = outfile;
}

void Logger_set_err_file(Logger *self, char *errfile) {
  self->err_file = errfile;
}

void Logger_set_cmd_size(Logger *self, int value) {
  self->cmd_size = value;
}

void Logger_destroy(Logger *self) {
  free(self);
}
