#include "command_processor.h"
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

void loop_pipe(char ***cmd, int redirect, char * redirect_file) 
{
  int   p[2];
  int err_p[2];
  pid_t pid;
  int   fd_in = 0;
  FILE * redirect_f;
  if (redirect) {
    redirect_f = fopen(redirect_file, "a+");
  }
  while (*cmd != NULL)
    {
      pipe(p);
      pipe(err_p);
      if ((pid = fork()) == -1)
        {
          exit(EXIT_FAILURE);
        }
      else if (pid == 0)
        {
          dup2(fd_in, 0); //change the input according to the old one 
          dup2(err_p[WRITE], 3); // error
          perror("~");
          if (*(cmd + 1) != NULL || redirect)
            dup2(p[1], 1);
          close(p[0]);
          execvp((*cmd)[0], *cmd);
          exit(EXIT_FAILURE);
        }
      else
        {
          wait(NULL);
          close(p[1]);
          close(err_p[1]);
          // temp part...
          char buff[1];
          read(err_p[0], &buff, 1);
          printf("%s", buff);
          close(err_p[0]);
          // end temp part

          if (redirect && *(cmd+1) == NULL) {
            // TODO: restore stdin...
            dup2(p[0], fileno(stdin));
            Util_write_to_file(redirect_f);
          }
          fd_in = p[0]; //save the input for the next command
          cmd++;
        }
    }
}

void Util_write_to_file(FILE * file) {
  char buffer[1];
  while (read(fileno(stdin), buffer, 1)) {
    fprintf(file, "%s", buffer);
  }
  fclose(file);
}
