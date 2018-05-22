#include "command_processor.h"

static void call_log_process(Shell *shell, int is_err, int pid, int code, char **command);

char ** Read_command(char * command, char * delimiter, int * count) {
  char * token;
  char * cmd;
  char ** retval = (char **)malloc(sizeof(char *)*strlen(command));
  int i = 1;
  
  cmd = strdup(command);
  token = strtok(cmd, delimiter);
  retval[0] = token;

  while (token = strtok(0, delimiter)) {  
    retval[i] = token;
    i++;
  }

  retval = realloc(retval, sizeof(char *) * i);
  *count = i; 
  return retval;
}

char *** parse_command_to_commands(char * command, char ** args) {
  perror("Error: function not yet implemented!");
  exit(-1);

  return NULL;
}

/**
 * @unused
**/
void Execute_single_command(char * command, char ** args, int redirect) {
  char *** commands = parse_command_to_commands(command, args);
  if (!redirect) {
    loop_pipe(commands, NULL, NULL);
  }
  else {
    /*
      TODO: IDEA!!! : use log process to redirect output too...
    */
    perror("Error: not yet implemented redirecting output...");
    exit(-1);
  }
}

void loop_pipe(char ***cmd, int redirect, char * redirect_file, Shell * shell) 
{
  int stdin_fd_cp;
  int stdout_fd_cp;
  int   p[2];
  pid_t pid;
  int code;
  int   fd_in = 0;
  int last_pid;
  FILE * redirect_f;

  stdin_fd_cp = dup(fileno(stdin));
  stdout_fd_cp = dup(fileno(stdout));
  if (redirect) {
    redirect_f = fopen(redirect_file, "a+");
  }
  while (*cmd != NULL)
    {
      pipe(p);

      char **log_args;
      if (strcmp(*cmd[0], LOG) == 0) {
        Shell_get_logger_str(log_args, 0, pid, code, *(cmd-1), &log_args);
      }
      if ((pid = fork()) == -1)
        {
          exit(EXIT_FAILURE);
        }
      else if (pid == 0)
        {
          dup2(fd_in, 0); //change the input according to the old one 
          if (*(cmd + 1) != NULL || redirect)
            dup2(p[1], 1);
          close(p[0]);
          if (strcmp((*cmd)[0] , LOG) == 0) {
            execvp(log_args[0], log_args);
            perror("logger failed.");
            exit(-1);
          }
          execvp((*cmd)[0], *cmd);
          fprintf(stderr, "process %s failed!",(*cmd[0]));
          exit(errno);
        }
      else
        {
          waitpid(&pid, &code, 0);
          char ** logger;
          
          Shell_get_logger_str(shell, 0, last_pid, code, *(cmd), &logger);
          close(p[1]);

          if (strcmp((*cmd)[0], LOG)) {
            last_pid = pid;
          }
          if (redirect && *(cmd+1) == NULL) {
            dup2(p[0], fileno(stdin));
            Util_write_to_file(redirect_f);
          }
          fd_in = p[0]; //save the input for the next command
          cmd++;
        }
    }

  dup2(stdin_fd_cp, fileno(stdin));
  dup2(stdout_fd_cp, fileno(stdout));
  fflush(stdout);
}

static void call_log_process(Shell *shell, int is_err, int pid, int code, char **command) {
  char **log_args;

  perror("I am going to call log processor (!!!) ");
  fprintf(stderr, "command is %s", command[0]);
  Shell_get_logger_str(log_args, is_err, pid, code, command, &log_args);
  perror("Running log process.\n");
  execvp(log_args[0], log_args);

  Shell_print_error(shell, "Error: logger process failed.\n", 1);
} 

void Util_write_to_file(FILE * file) {
  char buffer[1];
  while (read(fileno(stdin), buffer, 1)) {
    fprintf(file, "%s", buffer);
  }
  fclose(file);
}
