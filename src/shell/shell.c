#include "shell.h"

static int try_internal_cmds(Shell *self, char *command);
static char **Read_command(char *command, char *delimiter, int *size);
static void set_output_max_size(Shell *self, char *val);
static char **split_ignoring_char(char *command, char delimiter, 
                                  char ignore);

Shell *Shell_init() {
  Shell *retval; 

  retval = SHELL_ALLOC(1);
  retval->running_process_pids = NO_PROCESS;
  retval->config = ShellConfig_init();

  return retval;
}

void Shell_destroy(Shell *self) {
  ShellConfig_destroy(self->config);
  free(self);
  self = NULL;
}

void Shell_start(Shell *self) {
  Shell_main_loop(self);
}

void Shell_print(Shell *self) {
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  printf("%s> ", cwd);
  fflush(stdin);
}

void Shell_print_error(Shell *self, char *error, int do_exit) {
  fprintf(stderr, "%s%s%s\n", ANSI_COLOR_RED, error, ANSI_COLOR_RESET);

  if (do_exit) {
    Shell_exit(self, EXIT_FAILURE);
  }
}

//Execute the interactive shell
void Shell_main_loop(Shell *self) {
  char buffer[100];
  char *input;

  //Command handling
  char redirect_path[100];
  int command_count = 0;
  int is_redirect = 0;
  int has_pipe = 0;
  int is_async = 0;

  for (;;) {
    //Read input
    snprintf(buffer, sizeof(buffer), "%s:%s $ ", getenv("USER"), getcwd(NULL, 1024));
    fflush(stdout);
    input = readline(buffer);
    if (!input)
      break;

    //Try built in commands
    if (try_internal_cmds(self, input))
      continue;

    //Parse commands
    char ***commands = parse_line(input, &command_count,  
                                   &is_async, &has_pipe,
                                   &is_redirect, &redirect_path,
                                   "log", 3); // TODO: get log by arguments...

    self->executing_line = input;
    //Execute commands
    loop_pipe(commands, is_redirect, redirect_path, self); 
    //Free dynamics strings 
    free_commands(commands, command_count);
    free(input);
  }
}

//Get the logger call string
void Shell_get_logger_str(Shell *self, int is_err, int pid, int  code, char **cmd_args, char ***out) {
  char * str;
  int foo;

  ShellConfig_get_logger_call_string(self->config, is_err, pid, code, 
                                     self->executing_line, cmd_args, &str);
  *out = split_ignoring_char(str, ' ', '"');
}

void Shell_configure(Shell *self, int argc, char **argv) {
  int result;
  
  ArgsHandler *args_handler = ArgsHandler_init(6);
  //Print help and close the program
  ArgsHandler_insert_arguments(args_handler, Arg_init("-h", "--help", STRING(NULL), 
                               args_handler, 0, ArgsHandler_print)); 
  //Set the Logger output file
  ArgsHandler_insert_arguments(args_handler, Arg_init("-o", "--out-file", STRING(NULL), 
                               self->config, 1, ShellConfig_set_out_file)); 
  //Set the Logger error file
  ArgsHandler_insert_arguments(args_handler, Arg_init("-e", "--err-file", STRING(NULL), 
                               self->config, 1, ShellConfig_set_err_file)); 
  //Set if the Logger have to print the return code of the processe
  ArgsHandler_insert_arguments(args_handler, Arg_init("-c", "--code", STRING(NULL), 
                               self->config, 0, ShellConfig_enable_print_code)); 
  //Set the max size in characters of the output of every process
  ArgsHandler_insert_arguments(args_handler, Arg_init("-m", "--max-size", STRING(NULL), 
                               self, 1, set_output_max_size)); 
  //Set the logger process executable path
  ArgsHandler_insert_arguments(args_handler, Arg_init("-l", "--logger-path", STRING(NULL), 
                               self->config, 1, ShellConfig_set_logger_path)); 

  result = ArgsHandler_handle_input(args_handler, argc, argv);
  
  ArgsHandler_destroy(args_handler);
  
  if (result == -1) {
    Shell_print_error(self, "Invalid argument passing...", 1); 
  }
}

//Implementation of cd
void Shell_cd(Shell *self, char *new_dir) {
  /*char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  new_dir = strcat(cwd, new_dir);
*/
  if (NULL == new_dir) {
    if (NULL == (new_dir = getenv("HOME"))) {
      new_dir = getpwuid(getuid())->pw_dir;
      if (NULL == new_dir) {
        Shell_print_error(self, "Error: unable to find home folder.", 1);
      }
    }
  }
  int ret = chdir(new_dir);

 if (ret < 0) {
   Shell_print_error(self, "Error: cd failed", 0);
 }
}

void Shell_print_help(Shell *self) {
  printf("print command has not been implemented yet.\n");
}

void Shell_kill_running_process(Shell *self) {
  if (self->running_process_pids != NO_PROCESS)
    kill(self->running_process_pids, SIGKILL);
}

void Shell_exit(Shell *self, int code) {
  Shell_kill_running_process(self);
  printf("Shell exit with code: %d\n", code);
  Shell_destroy(self);
  exit(code);
}

//Manage the commands
static int try_internal_cmds(Shell *self, char *command) {
  int retval;
  int count;

  char **splitted;

  if (strcmp(command, "") == 0) {
    retval = 0;
  }
  else {
    if (strcmp(command, "quit") == 0) {
      Shell_exit(self, EXIT_SUCCESS);
    }

    splitted = Read_command(command, " ", &count);
    //String matching
    if (strcmp(splitted[0], "cd") == 0) {
      retval = 1;
      //cd takes only one argument
      if (count > 2) {
        Shell_print_error(self, "Error: cd takes only one argument", 0);
        //An error in cd should not trigger other errors.
        retval = 1;
      }
      else if(count == 1) {
        //Case cd without parameters.
        Shell_cd(self, NULL);     
      }
      else {
        Shell_cd(self, splitted[1]);
      }
    }
    else {
      retval = 0;
    }
    free(splitted);
  }
  return retval;
}

void Shell_on_quit_request(Shell *self, int signo) {
  if (self->running_process_pids == NO_PROCESS) {
    Shell_exit(self, 0);
  }
  else {
    Shell_kill_running_process(self);
  }
}

void Shell_set_running_process(Shell *self, int pid) {
  self->running_process_pids = pid;
}

static void set_output_max_size(Shell *self, char *val) {
  int m_size;

  m_size = atoi(val);

  ShellConfig_set_output_max_size(self->config, m_size);
}

//Ignore some char
static char **split_ignoring_char(char *command, char delimiter, 
                                  char ignore) {
  char **retval = (char **)malloc(sizeof(char *)*strlen(command));
  char curr;
  int last = 0;
  int check_enabled = 1;
  int index = 0;
  int i;
  for (i = 0; command[i] != EOF && command[i] != '\0'; i++) {
    curr = command[i];

    if (curr == ignore) {
     check_enabled = !check_enabled; 
    }
    
    if (check_enabled && curr == delimiter) {
      retval[index] = (char *)malloc(sizeof(char) * (i - last + 1)); 
      strncpy(retval[index], command + last, i - last); 
      retval[index][i-last] = '\0';
      index++;
      last = i + 1;
    }
  }

  if (last < i) {
    char *copy = (char *)malloc(sizeof(char) * (i - last + 1));
    strncpy(copy, command + last, i - last);
    copy[i-last] = '\0';
    retval[index] = copy;
    index++;
    last = i;
  }

  retval = (char **)realloc(retval, sizeof(char *) * (index+1));
  retval[index] = NULL;
  return retval;
}

//Read the command from the shell
static char **Read_command(char *command, char *delimiter, int *count) {
  char *token;
  char *cmd;
  char **retval = (char **)malloc(sizeof(char *)*strlen(command));
  int i = 1;
  
  cmd = strdup(command);
  token = strtok(cmd, delimiter);
  retval[0] = token;

  while (token = strtok(0, delimiter)) {  
    retval[i] = token;
    i++;
  }

  retval = realloc(retval, sizeof(char *) * (i + 1));
  *count = i; 
  //Array terminator
  retval[i] = NULL;
  return retval;
}
