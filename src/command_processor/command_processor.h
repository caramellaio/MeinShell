#define SPLIT_CHAR '|'
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "../shell/shell.h"
#include <wait.h>
#include <errno.h>

#define READ 0
#define WRITE 1
#define DELIMITERS "|"
#define LOG "log"
char **Read_command(char *command, char *delimiter, int *count);
