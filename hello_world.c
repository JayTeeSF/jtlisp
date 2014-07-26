#include <stdio.h>
#include <stdarg.h>
#include "jt_debug.h"

/**
 * -std=c99
 * tells the compiler which version of c to use
 * -Wall
 * tells the compiler to turn on "all" the "W"arning flags
 * cc -std=c99 -Wall hello_world.c -o hello_world
 *
 * For debugging on Mac use lldb (vs. gdb)
 * OR use valgrind
 *
 * ↪ ./hello_world 2>/dev/null
 * hello world
 *
 * ¿jthomas? ~/dev/myownlisp[master*]
 * ↪ ./hello_world 1>/dev/null
 * DEBUG: got 0 arg(s)
 * DEBUG: argv[0]: ./hello_world
 *
 * ¿jthomas? ~/dev/myownlisp[master*]
 * ↪ ./hello_world
 * DEBUG: got 0 arg(s)
 * DEBUG: argv[0]: ./hello_world
 * hello world
 **/

 // cc -Wall -std=c99 -o hello_world hello_world.c jt_debug.c
void print_greeting(char* greeting, char* entity) {
  printf("%s ", greeting);
  puts(entity);
}

int main(int argc, char** argv) {
  char *greeting, *entity;
  char default_greeting[] = "hello";
  char default_entity[] = "world";

  greeting = &default_greeting[0];
  entity = &default_entity[0];

  debug("argv[0]: %s got %d arg(s)\n", argv[0], argc -1);
  if (argc >= 2 ) { // arg 1 is the program
    debug("entity/argv[1]: %s\n", argv[1]);
    entity = argv[1];
    if (argc > 2) {
      debug("greeting/argv[2]: %s\n", argv[2]);
      greeting = argv[2];
    }
  }
  print_greeting(greeting, entity);
  return 0;
}
