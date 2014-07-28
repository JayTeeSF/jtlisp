#include "mpc.h"
#include "parser.h"
#include "jt_debug.h"

const int number_of_parsers = 13; //4 for RPN
mpc_result_t result;
mpc_parser_t* parsers[number_of_parsers]; // sadly, must be global to not become a dangler...
char* rpn_grammar = "                                                    \
                     number   : /-?[0-9]+/ ;                             \
                     operator : '+' | '-' | '*' | '/'  ;                 \
                     expr     : <number> | '(' <operator> <expr>+ ')' ;  \
                     lispy    : /^/ <operator> <expr>+ /$/  ;            \
                     ";

char* simple_speak_grammar = "                                                                                 \
                              article     : \"the\" | \"a\" | \"an\" ;                                         \
                              common_noun : \"dog\" | \"monkey\" | \"wind\" | \"house\" | \"president\" | <article> <common_noun> ;      \
                              proper_noun : \"joe\" | \"the white house\" | \"me\" ;                           \
                              noun : <common_noun> | <proper_noun> ;                                           \
                              conjunction : \"and\" | \"but\" | \"nor\" ;                                      \
                              preposition : \"on\" | \"around\" | \"in\" | \"to\" | \"for\" ;                  \
                              third_person_singular_verb : \"runs\" | \"sits\" | \"sways\" | \"breaks\" | \"gurgles\" ;  \
                              verb        : <third_person_singular_verb> ;                                     \
                              subject     : <noun>  ;                         \
                              object      : <noun>  ;         \
                              context     : <preposition>? <object> ;                           \
                              predicate   :  (<context>+ <conjunction>? <context>?)? | <predicate> <verb> ; \
                              lispy       : /^/ <subject> <verb> <predicate> /$/  ;                                   \
                              ";
                              // <context> | <conjuction>+ <verb> <predicate>+ ; 
                              // | <verb> <object> | <conjunction>? <predicate>+ ;                      
                              // | <verb> <context> <conjunction> <object> <predicate> | <verb> <conjunction> <object> <predicate> ;

void generate_parsers(void) {
  debug("parsers-size: %d ?= 0\n", sizeof(parsers));
  //parsers[0] = mpc_new("number");
  //parsers[1] = mpc_new("operator");
  //parsers[2] = mpc_new("expr");
  //parsers[3] = mpc_new("lispy"); // last one must be the main one
  //
  parsers[0] = mpc_new("article");
  parsers[1] = mpc_new("common_noun");
  parsers[2] = mpc_new("proper_noun");
  parsers[3] = mpc_new("noun");
  parsers[4] = mpc_new("conjunction");
  parsers[5] = mpc_new("preposition");
  parsers[6] = mpc_new("third_person_singular_verb");
  parsers[7] = mpc_new("verb");
  parsers[8] = mpc_new("subject");
  parsers[9] = mpc_new("object");
  parsers[10] = mpc_new("context");
  parsers[11] = mpc_new("predicate");
  parsers[12] = mpc_new("lispy");
  debug("parsers-size: %d ?> 0\n", sizeof(parsers));
  mpca_lang(
      MPCA_LANG_DEFAULT, grammar(SIMPLE_SPEAK),
      parsers[0],
      parsers[1],
      parsers[2],
      parsers[3], // end of RPN
      parsers[4],
      parsers[5],
      parsers[6],
      parsers[7],
      parsers[8],
      parsers[9],
      parsers[10],
      parsers[11],
      parsers[12]
  );
}

void cleanup_parsers(void) {
  // hmm... can't call mpc_cleanup 4x in a loop !?
  //mpc_cleanup(number_of_parsers, parsers[0], parsers[1], parsers[2], parsers[3]);
  mpc_cleanup(
      number_of_parsers,
      parsers[0],
      parsers[1],
      parsers[2],
      parsers[3], // end of RPN
      parsers[4],
      parsers[5],
      parsers[6],
      parsers[7],
      parsers[8],
      parsers[9],
      parsers[10],
      parsers[11],
      parsers[12]
  );
}

char* grammar(int key) {
  switch(key) {
    case RPN: return rpn_grammar;
    case SIMPLE_SPEAK: return simple_speak_grammar;
    case JTLISP: return rpn_grammar;
    default: return rpn_grammar;
  }
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
