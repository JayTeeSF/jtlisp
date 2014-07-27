#include "jt_debug.h"
#include <stdio.h>
#include <stdarg.h>

// create a library (.o file) by using the compiler's -c option:
// cc -Wall -std=c99 -o jt_debug.o -c jt_debug.c
// THEN to make a shared (dynamic) lib use -shared and prepend "lib" to the name used for the .o file
// cc -Wall -std=c99 -shared -o libjt_debug.so jt_debug.o

// http://www.ozzu.com/cpp-tutorials/tutorial-writing-custom-printf-wrapper-function-t89166.html
void debug(const char* format_string, ...) {
  fprintf(stderr, "DEBUG: ");
  va_list args; // name the ... argument
  va_start(args, format_string); // prepare args for use
  vfprintf(stderr, format_string, args); // use the version of printf that knows about args
  va_end(args); // clean-up
  fflush(stdout);
}

int number_of_arguments_in(int raw_argument_count) {
  int adjusted_argument_count = raw_argument_count - 1;
  return adjusted_argument_count >= 0 ? adjusted_argument_count : 0;
}

int number_of_valid_inputs(int number_of_arguments, int number_of_arguments_for_a_valid_input) {
  return ((number_of_arguments - number_of_arguments_for_a_valid_input) >= 0) ? (number_of_arguments / number_of_arguments_for_a_valid_input) : 0;
}
