#include "arg_handler.h"

ArgsHandler *ArgsHandler_init(int count) {
  ArgsHandler *self = (ArgsHandler *)malloc(sizeof(ArgsHandler));
  self->count = count;
  self->used = 0;
  self->arg_list = (Arg **)malloc(sizeof(Arg *) * self->count);
  return self;
}

void ArgsHandler_destroy(ArgsHandler *self) {
  int i;
  for (i = 0; i < self->count; i++)
    Arg_destroy(self->arg_list[i]);
  free(self);
}

/// <summary>Returns 1 if it find at least an arg, 0 if not and -1 if there is an error
/// <para name="self">ArgsHandler</para>
/// <para name="argc">Arguments count</para>
/// <para name="argv">Arguments vector</para>
/// </summary>
int ArgsHandler_handle_input(ArgsHandler *self, int argc, char **argv) {
  int retval = 0;
  //argv[0] is the program name
  int i;
  int j;
  for (i = 1; i < argc && retval != -1; i++) {
    int found = 0;
    char *arg_str;
    
    arg_str = argv[i];

    for (j = 0; j < self->count && found == 0 && retval != -1; j++) {
      Arg *arg = NULL;
      arg = self->arg_list[j];

      //Argument is not an argument name when it should
      if (!Arg_can_be_arg_name(arg_str)) {
        retval = -1;
        fprintf(stderr, "Error: %s is not an argument name.\n", arg_str);
        exit(-1);
      }
      else {
        if (Arg_strcmp(arg, argv[i])) {
          if (Arg_has_arg(arg)) {
            //USER INPUT error handling
            if (argc == i + 1) {
              retval = -1;
              printf("Error: %s requires a parameter but is the last string.\n", arg_str);
            }
            else if (Arg_can_be_arg_name(argv[i + 1])) {
              retval = -1;
              printf("Error: %s is a parameter name but has been handled as parameter value.\n", argv[i + 1]);
            }
            else {
              //Everything is fine
              arg->arg = argv[i + 1];
              //The next element in the list is the parameter
              i++;
            }
          }
          Arg_call_function(arg);
          retval = retval >= 0 ? 1 : retval;
          found = 1;
        }
      }
    }

    if (found == 0) {
      retval = -1;
      printf("Error: %s is not a valid parameter.\n", argv[i]);
    }
  }
  return retval;
}

int ArgsHandler_insert_arguments(ArgsHandler *self, Arg *arg) {
  if (self->used >= self->count)
    return FALSE;

  self->arg_list[self->used] = arg;
  self->used++;
  assert(self->used <= self->count);
  return TRUE;
}

void ArgsHandler_print(ArgsHandler *self) {
  int i;
  for (i = 0; i < self->count; i++) {
    Arg_print(self->arg_list[i]);
    printf("\n");
  }
}