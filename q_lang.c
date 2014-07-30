#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <stddef.h>
#include <editline/readline.h>
#include "jt_debug.h"

// cc -Wall -std=c99 -ledit jt_debug.c q_lang.c -o q_lang

int time_to_quit = 0;
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

int continuing(void) { //char* input) {
  return ! time_to_quit;
  //if (NULL == input) { return 1; }
  //return found_an_exit_in(input);
}

char* tokenize(char* input) {
  return strsep(&input, token_separator);
}

void free_all(int i, char** tokens) {
  for (i--; i>=0; i--){
    if (tokens[i]) {
      debug("freeing tokens[%d] => %s\n",i, tokens[i]);
      free(tokens[i]);
    } else {
      debug("already free'd tokens[%d] => %s\n",i, tokens[i]);
    }
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
  if (NULL == list) { debug("null list\n"); return NULL; }
  return list[0];
}

long add(char** operands) {
  char *operand;
  //char **operands_pointer = operands;
  long sum = 0;
  debug("adding to sum: %ld\n", sum);
  while((operand = head(operands)) != NULL) { // need a 'pop' operation
    operands = rest(operands);
    int operand_value = atoi(operand); // atof
    debug("operand_value: %d\n", operand_value);
    sum += operand_value;
  }
  debug("final sum: %ld\n", sum);
  return sum;
}

void eval(char** tokens) {
  char *operation = head(tokens);
  if (found_an_exit_in(operation)) { time_to_quit = 1; return; }
  if (strstr(operation, "add"))  { debug("gonna add\n"); printf("=> %ld\n", add(rest(tokens))); }
}

int main(int ac, char** av) {
  char *input = NULL;
  char *running;
  char **tokens = NULL;
  int i, loop_count = 0;
  debug("generating the prompt\n");
  char *prompt_string = generate_prompt();

  debug("intro-ing\n");
  intro();
  debug("starting loop\n");
  while(continuing()) { //head(tokens))) 
    loop_count++;
    if (loop_count > 1) {
      debug("free tokens\n");
      free(tokens); tokens = NULL;
    }

    i = 0;
    debug("reading line --using prompt: %s\n", prompt_string);
    input = readline(prompt_string);
    debug("adding input(%s) to history\n", input);
    add_history(input);

    tokens = malloc(sizeof(input) * sizeof(char*));
    //return strsep(&input, token_separator);
    //while ((tokens[i] = tokenize(input)) != NULL) 
    running = strdup(input);
    while ((tokens[i] = strsep(&running, token_separator)) != NULL) {
      debug("tokens[%d]: %s\n", i, tokens[i]);
      i++;
    }
    debug("evaluating the tokens\n");
    eval(tokens);

    //debug("freeing all %d tokens\n", i);
    //free_all(i, tokens);

    debug("freeing input\n");
    free(input);
    debug("freeing running\n");
    free(running);
    debug("looping\n");
  }
  //redundant...
  debug("free tokens\n");
  free(tokens); tokens = NULL;

  debug("freeing prompt_string\n");
  free(prompt_string);

  return 0;
}
