#include "prompt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <editline/readline.h>
/*
 * #include <editline/history.h>
 * static char input[2048]; // user input buffer
 **/

static char version[] = "0.1a";
static char minimal_prompt[] = "> ";

void welcome_message(void) {
  puts("Welcome to Jtlisp\n");
  // FIXME: type 'exit' or 'quit'
  puts("Press Ctrl+c to Exit\n");
}

void echo_message(char *message) {
  printf("No you're a %s", message);
}

/*
   void echo_prompt(void) {
   echo_message(prompt());
   }
   */

void respond_to(char *parsed_input) { 
  echo_message(parsed_input);
}

char *parse(char *input) { 
  return input;
}

void allocate_prompt(char *prompt_string) {
  int prompt_size = strlen(version) + strlen(minimal_prompt);
  prompt_string = malloc(prompt_size * sizeof(char*));
  bzero(prompt_string, prompt_size);
  strcat(prompt_string, version);
  strcat(prompt_string, "> ");
}

char *prompt(void) {
  /*
   * fputs("jtlisp> ", stdout);
   * return fgets(input, sizeof(input), stdin);
   **/
  char *prompt_string = NULL;
  allocate_prompt(prompt_string);

  char *input = readline(prompt_string);
  add_history(input);
  respond_to(parse(input));

  free(prompt_string);
  free(input);

  return input;
}
