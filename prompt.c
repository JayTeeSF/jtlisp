#include "prompt.h"
#include "jt_debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <editline/readline.h>

const char version[] = "0.1a";
const int exit_string_count = 2;
char *exit_strings[exit_string_count] = {"exit", "quit"};
const char minimal_prompt[] = "> ";

void welcome_message(void) {
  puts("Welcome to Jtlisp");
  puts("Type 'exit' or 'quit' (not including the single-quotes) to Exit");
}

void echo_message(char *message) {
  printf("No you're a %s\n", message);
}

int found(char *search_list[], char *key, int size) {
  for (int n = 0; n < size; ++n) {
    if (!strcmp(search_list[n],key)) {
      debug("n is %d\n", n);
      return 1;
    }
  }
  return 0;
}

int found_an_exit_in(char *input_token) {
  if (found(exit_strings, input_token, exit_string_count) > 0) {
    return 1;
  } else {
    return 0;
  }
}

// FIXME: allow for an many tokens
void respond_to(char *parsed_input) {
  if (found_an_exit_in(parsed_input)) {
    debug("found the exit\n");
    exit(0);
  } else {
    echo_message(parsed_input);
  }
}

char *parse(char *input) {
  return input;
}

char *generate_prompt(void) {
  int prompt_size = strlen(version) + strlen(minimal_prompt);
  char *prompt_string;
  prompt_string = malloc(prompt_size * sizeof(char*));
  bzero(prompt_string, prompt_size);
  strcat(prompt_string, version);
  strcat(prompt_string, "> ");
  return prompt_string;
}

void prompt(void) {
  char *prompt_string = generate_prompt();
  char *input = readline(prompt_string);
  add_history(input);

  respond_to(parse(input));
  free(prompt_string);
  free(input);
}
