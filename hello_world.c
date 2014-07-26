#include <stdio.h>
#include <stdarg.h>

/**
 * -std=c99
 * tells the compiler which version of c to use
 * -Wall
 * tells the compiler to turn on "all" the "W"arning flags
 * cc -std=c99 -Wall hello_world.c -o hello_world
 *
 * For debugging on Mac use lldb (vs. gdb)
 * OR use valgrind
 **/
void print_greeting(char* message) {
  puts(message);
}

void debug(const char* format_string, ...) {
  printf("DEBUG: ");
  va_list args; // name the ... argument
  va_start(args, format_string); // prepare args for use
  vprintf(format_string, args); // use the version of printf that knows about args
  va_end(args); // clean-up
}

int main(int argc, char** argv) {
  char* message;
  char default_message[] = "hello world";

  // printf("DEBUG: got %d arg(s)\n", argc -1);
  debug("got %d arg(s)\n", argc -1);
  if (argc > 1 ) { // arg 1 is the program
    debug("argv[1]: %s\n", argv[1]);
    message = argv[1];
  } else {
    debug("argv[0]: %s\n", argv[0]);
    message = &default_message[0];
  }
  print_greeting(message);
  return 0;
}
