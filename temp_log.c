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

  while ((value = read(fileno(stdin), buffer, BUFFER_SIZE)) != 0) {
    if (value < 0) {
      exit(-1);
    }
    printf("%s", buffer);
    fprintf(out, "%s", buffer);
  }

  fclose(out);
  return 0;
}
