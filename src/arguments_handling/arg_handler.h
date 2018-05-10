#ifndef __ARG_HANDLER__
#define __ARG_HANDLER__

#define TRUE 1
#define FALSE 0

#include "arg.h"
#include "string.h"

typedef struct ArgsHandler {
  Arg ** arg_list;
  int count;
  int used;
} ArgsHandler;

ArgsHandler * ArgsHandler_init(int count);
void ArgsHandler_destroy(ArgsHandler * self);

int ArgsHandler_handle_input(ArgsHandler * self, int argc, char ** argv);

int ArgsHandler_insert_arguments(); // fsldjfsusd

void ArgsHandler_print(ArgsHandler * self);
#endif
