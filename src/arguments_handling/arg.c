#include "arg.h"

Arg * Arg_init(char * short_name, char *name, void *arg, void * caller, int has_arg, Func func) {
  Arg * self = (Arg *)malloc(sizeof(Arg));
  self->short_name = short_name;
  self->name = name;
  self->caller = caller;
  self->arg = arg;
  self->has_arg = has_arg;
  self->func = func;

  return self;
}

void Arg_destroy(Arg * self) {
  free(self);
}

char * Arg_get_name(Arg * self) {
  return self->name;
}

char * Arg_get_short_name(Arg * self) {
  return self->short_name;
}

int Arg_get_count(Arg * self) {
  return self->has_arg;
}

void Arg_call_function(Arg *self) {
  self->func(self->caller, self->arg);
}
void Arg_print(Arg * self) {
  printf("option %s | %s, has_arg=%d", self->name,
         self->short_name, self->has_arg);
}

int Arg_strcmp(Arg * self, char * str) {
  return strcmp(self->name, str) == 0 ||
    strcmp(self->short_name, str) == 0;
}

int Arg_can_be_arg_name(char * str_arg) {
  return str_arg[0] == '-';
}
