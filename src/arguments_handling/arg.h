#ifndef __ARG__H__
#define __ARG__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef void (*Func)(void*, void*);

typedef struct Arg {
  char * short_name;
  char * name;
  void * arg;
  void * caller;
  int has_arg;
  Func func;
} Arg;

Arg * Arg_init(char * short_name, char *name, void *arg, void * caller, int has_arg, Func func);
void Arg_destroy(Arg * self);

char * Arg_get_name(Arg * self);
char * Arg_get_short_name(Arg * self);
int Arg_get_count(Arg * self);

void Arg_print(Arg * self);
void Arg_call_function(Arg *self);
int Arg_strcmp(Arg * self, char * str);
int Arg_can_be_arg_name(char * str_arg);
#endif
