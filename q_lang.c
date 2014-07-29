#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <editline/readline.h>
#include "jt_debug.h"

// cc -Wall -std=c99 -ledit jt_debug.c q_lang.c -o q_lang

char version[] = "0.01a";
char token_separator[] = " ";
char minimal_prompt[] = " > ";
const int exit_string_count = 2;
char *exit_strings[exit_string_count] = {"exit", "quit"};

void intro(void) {
  puts("Welcome to Jtlisp");
  puts("Type 'exit' or 'quit' to Exit");
}

int found(char *search_list[], char *key, int size) {
  for (int n = 0; n < size; ++n) {
    if (!strcmp(search_list[n],key)) {
      return 1;
    }
  }
  return 0;
}

char *generate_prompt(void) {
  int prompt_size = strlen(version) + strlen(minimal_prompt + 1);
  char *prompt_string;
  prompt_string = malloc(prompt_size * sizeof(char*));
  bzero(prompt_string, prompt_size);
  strcat(prompt_string, version);
  strcat(prompt_string, minimal_prompt);
  return prompt_string;
}

int found_an_exit_in(char *input_token) {
  if (found(exit_strings, input_token, exit_string_count) > 0) {
    return 1;
  } else {
    return 0;
  }
}

int continuing(char* input) {
  if (input == NULL) { return 1; }
  return found_an_exit_in(input);
}

char* tokenize(char* input) {
  return strsep(&input, token_separator);
}

void free_all(int i, char** tokens) {
  for (;i>0; i--){
    free(tokens[i]);
  }
}

// using a char** 
// IF I want to remove an element from the middle of a list
// I have to move all the pointers!
// At that point
// a better solution is to switch to a data-structure (i.e. doubly-linked list)
char** rest(char** list) {
  //list[0] wonder if I need to "free" the first string!?
  return &list[1]; // perhaps just moving the head of the list will work...
}

char* head(char** list) {
  if (list == NULL) { return NULL; }
  return list[0];
}

long add(char** operands) {
  char *operand;
  long sum = 0;
  while((operand = head(operands)) != NULL) {
    sum += atof(operand);
  }
  return sum;
}

void eval(char** tokens) {
  char *operation = head(tokens);
  if (strstr(operation, "add"))  { printf("=> %ld\n", add(rest(tokens))); }
}

int main(int ac, char** av) {
  char *input = NULL;
  char **tokens = NULL;
  int i = 0;
  debug("generating the prompt\n");
  char *prompt_string = generate_prompt();

  debug("intro-ing\n");
  intro();
  debug("starting loop\n");
  while(continuing(head(tokens))) {
    debug("reading line --using prompt: %s\n", prompt_string);
    input = readline(prompt_string);
    debug("adding input(%s) to history\n", input);
    add_history(input);

    tokens = malloc(sizeof(input) * sizeof(char*));
    while ((tokens[i] = tokenize(input)) != NULL) {
      debug("token[%d]: %s\n", i, tokens[i]);
      i++;
    }
    debug("evaluating the tokens\n");
    eval(tokens);

    debug("freeing all %d tokens\n", i);
    free_all(i, tokens);
    free(tokens);
    debug("freeing input\n");
    free(input);
  }

  debug("freeing prompt_string\n");
  free(prompt_string);

  return 0;
}
