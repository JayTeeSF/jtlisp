#include "mpc.h"
#include "parser.h"
#include "jt_debug.h"

const int number_of_parsers = 4;
mpc_result_t result;
mpc_parser_t* parsers[number_of_parsers]; // sadly, must be global to not become a dangler...
char* grammar = "                                                    \
                 number   : /-?[0-9]+/ ;                             \
                 operator : '+' | '-' | '*' | '/'  ;                 \
                 expr     : <number> | '(' <operator> <expr>+ ')' ;  \
                 lispy    : /^/ <operator> <expr>+ /$/  ;            \
                 ";

void cleanup_parsers(void) {
  mpc_cleanup(number_of_parsers, parsers[0], parsers[1], parsers[2], parsers[3]);
  /*
     for(int i=0; i<number_of_parsers; i++) {
     mpc_cleanup(1, parsers[i]);
     }
   **/
}

void generate_parsers(void) {
  debug("parsers-size: %d ?= 0\n", sizeof(parsers));
  parsers[0] = mpc_new("number");
  parsers[1] = mpc_new("operator");
  parsers[2] = mpc_new("expr");
  parsers[3] = mpc_new("lispy"); // last one must be the main one
  debug("parsers-size: %d ?> 0\n", sizeof(parsers));
  mpca_lang(MPCA_LANG_DEFAULT, grammar, parsers[0], parsers[1], parsers[2], parsers[3]);
}

int parse(char *input) {
  debug("calling mpc_parse\n");
  if (mpc_parse("<stdin>", input, parsers[number_of_parsers - 1], &result)) {
    debug("returning result\n");
    return 1;
  } else {
    debug("borked result\n");
    return 0;
  }
}

// HMM: allow for an many tokens
//void respond_to(char *parsed_input)
void respond_to(int result_status) {
  mpc_result_t *r = &result;
  debug("starting respond_to...\n");
  if (result_status) {
    debug("output not NULL\n");
    mpc_ast_print(r->output);
    debug("did you see mpc_ast_print?!\n");
    mpc_ast_delete(r->output);
  } else {
    debug("output was NULL\n");
    mpc_err_print(r->error);
    debug("did you see mpc_err_print?!\n");
    mpc_err_delete(r->error);
  }
  /*
     if (found_an_exit_in(parsed_input)) {
     debug("found the exit\n");
     exit(0);
     } else {
     echo_message(parsed_input);
     }
     */
  debug("done respond_to\n");
}
