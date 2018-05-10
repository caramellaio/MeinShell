#include "log.h"

static void log_header(Logger * self, char * process, int pid, FILE * file);
static void log_output(Logger * self, int fd, FILE * file);

Logger * Logger_init() {
  Logger * retval; 
  retval = (Logger *)malloc(sizeof(Logger));

  return retval;
}
Logger * Logger_create(char * out_file, char *err_file, int print_code, int cmd_size) {
  Logger * retval;

  retval = Logger_init();
  retval->out_file = out_file;
  retval->err_file = err_file;
  retval->print_code = print_code;
  retval->cmd_size = cmd_size;

  return retval;
}

void Logger_destroy(Logger * self) {
  if (self) {
    free(self);
  }
}

//TODO: doesn't log code even if option is enabled
int Logger_log(Logger * self, char * process, int pid, int err_fd, int out_fd, int code) {
  if (err_fd == out_fd)
    /* caso stesso output */  ;
  FILE * out = fopen(self->out_file, "a+");
  FILE * err = fopen(self->err_file, "a+");
  
  log_header(self, process, pid, out);
  log_output(self, out_fd, out);
  fclose(out);
  close(out_fd);

  log_header(self, process, pid, err);
  log_output(self, err_fd, err);
  fclose(err);
  close(err_fd);

  return 0;
}

int Logger_clean(Logger * self) {
  int out_file_removed = remove(self->out_file);
  int err_file_removed = remove(self->err_file);
  // TODO: could follow the standard 0=ok < 0 not ok
  return out_file_removed == 0 && err_file_removed == 0;
}

void Logger_set_out_file(Logger * self, char * value) {
  self->out_file = value;
}

void Logger_set_err_file(Logger * self, char * value) {
  self->err_file = value;
}

void Logger_enable_print_code(Logger * self) {
  self->print_code = TRUE;
}

void Logger_set_cmd_size(Logger * self, int value) {
  self->cmd_size = value;
}

static void log_header(Logger * self, char * process, int pid, FILE * file) {
  time_t now = time(NULL);

  char buffer[100];
  strftime(buffer, 100, "%Y-%m-%d %H:%M:%S", localtime(&now));
  fprintf(file, "SHELL ID: %d\n", self->id_count);
  fprintf(file, "PID: %d\n", pid);
  fprintf(file, "COMMAND: %s\n", process);
  fprintf(file, "SUB COMMAND: %s\n", process); // not mandatory
  fprintf(file, "DATE: %s\n", buffer);
  fprintf(file, "OUTPUT:");
}

static void log_output(Logger * self, int fd, FILE * file) {
  char buffer[BUFFER_SIZE];
  int val = 1;
  while (val != 0) {
    val =  read(fd, buffer, BUFFER_SIZE);
    if (val < 0) {
      // use something different for error logging.
      printf("Error: read failed from fd:%d", fd);
      // TODO: use something better than an exit.
      exit(-1);
    }
    fprintf(file,buffer);
  }
}
