#include "prompt.h"
#include "jt_debug.h"

// cc -Wall -std=c99 -o jtlisp -ledit jtlisp.c prompt.c jt_debug.c
int main(int ac, char** av) {
  welcome_message();
  while(1) {
    prompt();
  }

  return 0;
}
