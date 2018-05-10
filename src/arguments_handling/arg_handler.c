
#include "arg_handler.h"

ArgsHandler * ArgsHandler_init(int count) {
  ArgsHandler *self = (ArgsHandler *)malloc(sizeof(ArgsHandler));
  self->count = count;
  self->used = 0;
  self->arg_list = (Arg **)malloc(sizeof(Arg *)*self->count);



}
void ArgsHandler_destroy(ArgsHandler * self) {
  for (int i = 0; i < self->count; i++) {
    Arg_destroy(self->arg_list[i]);
  }

  free(self);
}

int ArgsHandler_handle_input(ArgsHandler * self, int argc, char ** argv) {
 
  int found = FALSE;
  // argv[0] is the program name
  for (int i = 1; i < argc; i++) {

    for (int j = 0; j < self->count && !found; j++) {
      Arg * arg = self->arg_list[j];

      if (Arg_strcmp(arg, argv[i])) {
        // TODO: use a getter
        if (arg->has_arg) {
          // probably wrong.. 
          
          // TODO:if an arg has parameters and is the last 
          // get segfault
          
          if (i+1 == argc)
            return FALSE; // horrible workaround
          if (Arg_can_be_arg_name(argv[i+1])) {
            printf("warning %s is a parameter name but has been handled as parameter value.\n",
                   argv[i+1]);
          }
          arg->arg = argv[i+1];
          // the next element in the list is the parameter
          i++;
        }
        Arg_call_function(arg);
        found = TRUE;
      }
    }
  }

  // TODO: choose between 0 success -1 fail and 
  //                      1 success  0 fail..
  return found;
}

int ArgsHandler_insert_arguments(ArgsHandler * self, Arg * arg) {
  if (self->used >= self->count)
    return FALSE;

  self->arg_list[self->used] = arg;
  self->used++;

  return TRUE;
} 

void ArgsHandler_print(void * args) {
  ArgsHandler *self = (ArgsHandler *)args;
  
  for (int i = 0; i < self->count; i++) {
    Arg_print(self->arg_list[i]);
    printf("\n");
  }  
}

