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

int get_p_i(int n) {
  if (n == 0)
    n--;
  return 2 * n +1;
}

int get_p_o(int n) {
  if (n == 0)
    n--;
  return 2 * n + 3;
}

int get_l_i(int n) {
  if (n == 0)
    n--;
  return 2 * n + 2;
}

int get_l_o(int n) {
  if (n == 0)
    n--;
  return 2 * n + 4;
}

void Execute_commands_with_pipe(char ** commands, int count) {

  int pipes_count = 4 * count -1; // with stdout, stdin
  int  fds[pipes_count];

  for (int i = 0; i < pipes_count; i+= 2) {
    pipe(fds+i);
  }

  for(int i =0; i < count; i++) {
    char * proc_name = commands[i];
    char * cmd_name;
    char ** cmd_args;
    int tmp;
    int pid;

    int log_i = fds[get_l_i(i)];
    int log_o = fds[get_l_o(i)];
    int p_i;
    int p_o = fds[get_p_o(i)] ;

    cmd_args = Read_command(proc_name, " ", &tmp);
    cmd_name = cmd_args[0];

    
    
    if ((pid = fork()) == 0) {
      if (i > 0) {
        p_i = fds[get_p_i(i)];
        if (dup2(p_i, fileno(stdin)) < 0)
          perror("dup2 p_i failed\n"); 
      }

      if (dup2(p_o, fileno(stdout)) < 0)
        perror("dup2 p_o failed\n");

      close(p_i);
      close(p_o);
      // try read here...
      execvp(cmd_name, cmd_args);
      perror("Error..");
    }
    
    int code;
    waitpid(pid, &code, 0);
      //char ** aa = Read_command("wc", " ", NULL);
      //execvp(aa[0], aa);
      //perror("wat");
    if (pid = fork() == 0) {
      dup2(log_i, STDIN_FILENO);
      if (i > 0)
        close(p_i);
      close(p_o);
      close(log_i);
      close(log_o);
      duplicate_out(STDIN_FILENO, log_o, fileno(stderr));
      exit(0);
    }
    /*
     Logger_log(fd[WRITE],....)
     *
    */
  }
  

  wait(NULL);
}

void close_all_pipes(int * pipes, int count) {
  for (int i =0; i < count; i++) {
    if(close(pipes[i]) < 0)
      perror("unable to close pipes!");
  }
}

/*
int * do_pipe(char *command0, char ** command0_args, char *command1, char ** command1_args, int * fd, int executed) {
  int pid0, pid1;
  if (! executed) {
    if ( (pid0 = fork()) == 0) {
      set_as_parent(fd);
      execvp(command0, command0_args);
      perror("unable to launch process");
    }
  }
  
  int fd_o[2];
  set_as_child(fd);
  duplicate_out(fd[0], stdout, fd[1]);
  if ((pid1 = fork()) == 0) {
    set_as_child(fd);
    execvp(command1, command1_args);
    perror("failed to execute son");
  }

  return NULL;
}*/
