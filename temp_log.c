#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 80

/*
  SETUP FOR TEMP LOGGER
  gcc temp_log.c -o log
  sudo cp log /usr/bin/log
  
  the log file is log.txt (this program does not takes arguments)
*/
int main(int argc, char **argv) {
  int value = 1;
  char buffer[BUFFER_SIZE];
  FILE *out = fopen("log.txt", "a+");

  /*while ((value = read(fileno(stdin), buffer, BUFFER_SIZE)) != 0) {
    if (value < 0) {
      perror("Error: unable to read from stdin.");
      exit(-1);
    }
    printf("%s", buffer);
    fprintf(file, "%s", buffer);
  }*/
  while ( (value = read(fileno(stdin), buffer, BUFFER_SIZE)) != 0){
    if (value < 0) {
      perror("Error: unable to read from stdin.");
      exit(-1);
    }
    char *str = (char*)malloc(value+1);
    strncpy(str, buffer, value);
    str[value+1]='\0';  
    write(fileno(stdout),str,value+1);
    free(str);
  }

  fclose(out);
  return 0;
}
