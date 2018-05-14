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

void Execute_commands_with_pipe(char ** commands, int count) {
  int fd[2];

  char * first_cmd;
  char ** first_cmd_args;
  int tmp;
  first_cmd_args = Read_command(commands[0], " ", &tmp);
  first_cmd = first_cmd_args[0];
  pipe(fd);
  int pid;
  if ((pid = fork()) == 0) {
    set_as_parent(fd);
    execvp(first_cmd, first_cmd_args);
    perror("Error...");
  }
  
  waitpid(pid, NULL, 0);

  for(int i =1; i < count; i++) {
    /* probably a second pipe for each iteration is necessary... */
    char * proc_name = commands[i];
    char * cmd_name;
    char ** cmd_args;
    int tmp;
    int pid;

    cmd_args = Read_command(proc_name, " ", &tmp);
    cmd_name = cmd_args[0];
    if (i < count-1) {
      set_as_middle(fd);
    }
    else {
      set_as_child(fd);
    }
    
    if ((pid = fork()) == 0) {
      execvp(cmd_name, cmd_args);
      perror("Error..");
    }
    /*
     Logger_log(fd[WRITE],....)
     *
    */
    waitpid(pid, NULL, 0);
  }

  wait(NULL);
}



void set_as_child(int * fd) {
  close(fd[WRITE]);
  printf("fd[READ]=%d\n", fd[READ]);
  if (dup2(fd[READ], STDIN_FILENO) < 0) {
    perror("failed to set as child..\n");
    exit(-1);
  }
  close(fd[READ]);
}

void set_as_middle(int * fd) {
  dup2(fd[READ], STDIN_FILENO);
  close(fd[READ]);
  dup2(fd[WRITE], STDOUT_FILENO);
  close(fd[WRITE]);
}

void set_as_parent(int * fd) {
  close(fd[READ]);
  if (dup2(fd[WRITE], STDOUT_FILENO) < 0) {
    perror("failed to set as parent..\n");
    exit(-1);
  }
  close(fd[WRITE]);
}
