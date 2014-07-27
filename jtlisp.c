#include "mpc.h"
#include "parser.h"
#include "prompt.h"
#include "jt_debug.h"

// cc -Wall -std=c99 -o jtlisp -ledit jtlisp.c mpc.c prompt.c parser.c jt_debug.c
int main(int ac, char** av) {
  generate_parsers();
  welcome_message();

    debug("prompt...\n");
  while( prompt() ) {
    // (!found_an_exit_in(parsed_input)) 
  }

  debug("cleaning up parser...\n");
  // undefine and delete the parsers
  cleanup_parsers();

  return 0;
}
