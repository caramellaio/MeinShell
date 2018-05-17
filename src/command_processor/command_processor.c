#include "command_processor.h"

int stdin_cp;
int stdout_cp;

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


/*void CommandProcessor_run_command(Command * command) {
  int do_redirect =0;
  char * redirect;
  if (command->type == REDIRECT) {
    do_redirect = 1;
    // not so sure... 
    redirect = (char *)command->right;
  }
  else if (command->type == PIPE) {
    // command should terminate with a NULL
    char *** commands = CommandReader_parse_pipe(command); 
    loop_pipe(commands);
  }
  else {
    // SIMPLE COMMAND 
    Execute_single_command(Command_get_command_string(command),
                           Command_get_param(command));
  }
}*/

char *** parse_command_to_commands(char * command, char ** args) {
  perror("Error: function not yet implemented!");
  exit(-1);

  return NULL;
}
void Execute_single_command(char * command, char ** args, int redirect) {
  char *** commands = parse_command_to_commands(command, args);
  if (!redirect) {
    loop_pipe(commands);
  }
  else {
    /*
      TODO: IDEA!!! : use log process to redirect output too...
    */
    perror("Error: not yet implemented redirecting output...");
    exit(-1);
  }
}

void loop_pipe(char ***cmd) 
{
  int   p[2];
  pid_t pid;
  int   fd_in = 0;

  while (*cmd != NULL)
    {
      pipe(p);
      if ((pid = fork()) == -1)
        {
          exit(EXIT_FAILURE);
        }
      else if (pid == 0)
        {
          dup2(fd_in, 0); //change the input according to the old one 
          if (*(cmd + 1) != NULL)
            dup2(p[1], 1);
          close(p[0]);
          execvp((*cmd)[0], *cmd);
          exit(EXIT_FAILURE);
        }
      else
        {
          wait(NULL);
          close(p[1]);
          fd_in = p[0]; //save the input for the next command
          cmd++;
        }
    }
}
