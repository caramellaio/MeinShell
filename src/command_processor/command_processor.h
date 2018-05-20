#define SPLIT_CHAR '|'
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "../log/log.h"
#define READ 0
#define WRITE 1
#define DELIMITERS "|"
char **Read_command(char *command, char *delimiter, int *count);
