#include "mpc.h"
#include "parser.h"
#include "jt_debug.h"

const int number_of_parsers = 6; //13 SIMPLE; //4 for RPN
mpc_result_t result;
mpc_parser_t* parsers[number_of_parsers]; // sadly, must be global to not become a dangler...

/* Add QEXPR as possible lval type */
enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_SEXPR, LVAL_QEXPR };


    //case JTLISP: return jtlisp_grammar;
char* jtlisp_grammar = "                                                  \
                     number   : /-?[0-9]+/ ;                              \
                     symbol   : \"list\" | \"head\" | \"tail\" | \"eval\" | \"join\" | '+' | '-' | '*' | '/'  ;                 \
                     sexpr    : '(' <expr>* ')' ; \
                     qexpr    : '{' <expr>* '}' ; \
                     expr     : <number> | <symbol> | <sexpr> | <qexpr> ; \
                     lispy    : /^/ <expr>* /$/  ;             \
                     ";

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
  //debug("parsers-size: %d ?= 0\n", sizeof(parsers));
  parsers[0] = mpc_new("number");
  parsers[1] = mpc_new("symbol");
  parsers[2] = mpc_new("sexpr");
  parsers[3] = mpc_new("qexpr");
  parsers[4] = mpc_new("expr");
  parsers[5] = mpc_new("lispy"); // last one must be the main one

  /**
   * parsers[0] = mpc_new("number");
   * parsers[1] = mpc_new("operator");
   * parsers[2] = mpc_new("expr");
   * parsers[3] = mpc_new("lispy"); // last one must be the main one
   */
  /**
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
   */
  //debug("parsers-size: %d ?> 0\n", sizeof(parsers));
  mpca_lang(
      MPCA_LANG_DEFAULT, grammar(JTLISP),
      parsers[0],
      parsers[1],
      parsers[2],
      parsers[3],
      parsers[4],
      parsers[5]
      );
      /*
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
   */
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
      parsers[5]
      //,
      //parsers[6],
      //parsers[7],
      //parsers[8],
      //parsers[9],
      //parsers[10],
      //parsers[11],
      //parsers[12]
      );
}

char* grammar(int key) {
  switch(key) {
    case RPN: return rpn_grammar;
    case SIMPLE_SPEAK: return simple_speak_grammar;
    case JTLISP: return jtlisp_grammar;
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
  //debug("starting respond_to...\n");
  if (result_status) {
    //debug("output not NULL\n");
    //mpc_ast_print(r->output);
    lval *x = lval_eval(lval_read(r->output));
    printf("=> ");
    lval_println(x);
    lval_del(x);

    mpc_ast_delete(r->output);
  } else {
    //debug("output was NULL\n");
    mpc_err_print(r->error);
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
  //debug("done respond_to\n");
}

// ############### BEGIN LVAL TYPES
lval* lval_num(long x) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_NUM;
  v->num = x;
  return v;
}

lval* lval_err(char* m) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_ERR;
  v->err = malloc(strlen(m) + 1);
  strcpy(v->err, m);
  return v;
}

lval* lval_sym(char* s) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SYM;
  v->sym = malloc(strlen(s) + 1);
  strcpy(v->sym, s);
  return v;
}

lval* lval_sexpr(void) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SEXPR;
  v->count = 0;
  v->cell = NULL;
  return v;
}

/* A pointer to a new empty Qexpr lval */
lval* lval_qexpr(void) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_QEXPR;
  v->count = 0;
  v->cell = NULL;
  return v;
}

// ############### END LVAL TYPES


// ############### BEGIN LVAL METHODS
void lval_del(lval* v) {

  switch (v->type) {
    case LVAL_NUM: break;
    case LVAL_ERR: free(v->err); break;
    case LVAL_SYM: free(v->sym); break;

                   /* If Qexpr or Sexpr then delete all elements inside */
    case LVAL_QEXPR:
    case LVAL_SEXPR:
                   for (int i = 0; i < v->count; i++) {
                     lval_del(v->cell[i]);
                   }
                   /* Also free the memory allocated to contain the pointers */
                   free(v->cell);
                   break;
  }

  free(v);
}

lval* lval_add(lval* v, lval* x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(lval*) * v->count);
  v->cell[v->count-1] = x;
  return v;
}

lval* lval_pop(lval* v, int i) {
  lval* x = v->cell[i];
  memmove(&v->cell[i], &v->cell[i+1], sizeof(lval*) * (v->count-i-1));
  v->count--;
  v->cell = realloc(v->cell, sizeof(lval*) * v->count);
  return x;
}

lval* lval_join(lval* x, lval* y) {

  while (y->count) {
    x = lval_add(x, lval_pop(y, 0));
  }

  lval_del(y);
  return x;
}

lval* lval_take(lval* v, int i) {
  lval* x = lval_pop(v, i);
  lval_del(v);
  return x;
}


void lval_expr_print(lval* v, char open, char close) {
  putchar(open);
  for (int i = 0; i < v->count; i++) {

    lval_print(v->cell[i]);

    if (i != (v->count-1)) {
      putchar(' ');
    }
  }
  putchar(close);
}

void lval_print(lval* v) {
  switch (v->type) {
    case LVAL_NUM:   printf("%li", v->num); break;
    case LVAL_ERR:   printf("Error: %s", v->err); break;
    case LVAL_SYM:   printf("%s", v->sym); break;
    case LVAL_SEXPR: lval_expr_print(v, '(', ')'); break;
    case LVAL_QEXPR: lval_expr_print(v, '{', '}'); break;
  }
}

void lval_println(lval* v) { lval_print(v); putchar('\n'); }

// shorten syntax with macro:
#define LASSERT(args, cond, err) if (!(cond)) { lval_del(args); return lval_err(err); }


lval* builtin_list(lval* a) {
  a->type = LVAL_QEXPR;
  return a;
}

lval* builtin_head(lval* a) {
  LASSERT(a, (a->count == 1                 ), "Function 'head' passed too many arguments.");
  LASSERT(a, (a->cell[0]->type == LVAL_QEXPR), "Function 'head' passed incorrect type.");
  LASSERT(a, (a->cell[0]->count != 0        ), "Function 'head' passed {}.");

  lval* v = lval_take(a, 0);
  while (v->count > 1) { lval_del(lval_pop(v, 1)); }
  return v;
}

lval* builtin_tail(lval* a) {
  LASSERT(a, (a->count == 1                 ), "Function 'tail' passed too many arguments.");
  LASSERT(a, (a->cell[0]->type == LVAL_QEXPR), "Function 'tail' passed incorrect type.");
  LASSERT(a, (a->cell[0]->count != 0        ), "Function 'tail' passed {}.");

  lval* v = lval_take(a, 0);
  lval_del(lval_pop(v, 0));
  return v;
}

lval* builtin_eval(lval* a) {
  LASSERT(a, (a->count == 1                 ), "Function 'eval' passed too many arguments.");
  LASSERT(a, (a->cell[0]->type == LVAL_QEXPR), "Function 'eval' passed incorrect type.");

  lval* x = lval_take(a, 0);
  x->type = LVAL_SEXPR;
  return lval_eval(x);
}

lval* builtin_join(lval* a) {

  for (int i = 0; i < a->count; i++) {
    LASSERT(a, (a->cell[i]->type == LVAL_QEXPR), "Function 'join' passed incorrect type.");
  }

  lval* x = lval_pop(a, 0);

  while (a->count) {
    x = lval_join(x, lval_pop(a, 0));
  }

  lval_del(a);
  return x;
}

lval* builtin_op(lval* a, char* op) {

  for (int i = 0; i < a->count; i++) {
    if (a->cell[i]->type != LVAL_NUM) {
      lval_del(a);
      return lval_err("Cannot operate on non-number!");
    }
  }

  lval* x = lval_pop(a, 0);
  if ((strcmp(op, "-") == 0) && a->count == 0) { x->num = -x->num; }

  while (a->count > 0) {

    lval* y = lval_pop(a, 0);

    if (strcmp(op, "+") == 0) { x->num += y->num; }
    if (strcmp(op, "-") == 0) { x->num -= y->num; }
    if (strcmp(op, "*") == 0) { x->num *= y->num; }
    if (strcmp(op, "/") == 0) {
      if (y->num == 0) {
        lval_del(x); lval_del(y);
        x = lval_err("Division By Zero.");
        break;
      }
      x->num /= y->num;
    }

    lval_del(y);
  }

  lval_del(a);
  return x;
}

lval* builtin(lval* a, char* func) {
  if (strcmp("list", func) == 0) { return builtin_list(a); }
  if (strcmp("head", func) == 0) { return builtin_head(a); }
  if (strcmp("tail", func) == 0) { return builtin_tail(a); }
  if (strcmp("join", func) == 0) { return builtin_join(a); }
  if (strcmp("eval", func) == 0) { return builtin_eval(a); }
  if (strstr("+-/*", func)) { return builtin_op(a, func); }
  lval_del(a);
  return lval_err("Unknown Function!");
}

lval* lval_eval_sexpr(lval* v) {

  for (int i = 0; i < v->count; i++) {
    v->cell[i] = lval_eval(v->cell[i]);
  }

  for (int i = 0; i < v->count; i++) {
    if (v->cell[i]->type == LVAL_ERR) { return lval_take(v, i); }
  }

  if (v->count == 0) { return v; }

  if (v->count == 1) { return lval_take(v, 0); }

  lval* f = lval_pop(v, 0);
  if (f->type != LVAL_SYM) {
    lval_del(f); lval_del(v);
    return lval_err("S-expression Does not start with symbol.");
  }

  /* Call builtin with operator */
  lval* result = builtin(v, f->sym);
  lval_del(f);
  return result;
}

lval* lval_eval(lval* v) {
  if (v->type == LVAL_SEXPR) { return lval_eval_sexpr(v); }
  return v;
}

lval* lval_read_num(mpc_ast_t* t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno != ERANGE ? lval_num(x) : lval_err("invalid number");
}

lval* lval_read(mpc_ast_t* t) {

  if (strstr(t->tag, "number")) { return lval_read_num(t); }
  if (strstr(t->tag, "symbol")) { return lval_sym(t->contents); }

  lval* x = NULL;
  if (strcmp(t->tag, ">") == 0) { x = lval_sexpr(); }
  if (strstr(t->tag, "sexpr"))  { x = lval_sexpr(); }
  if (strstr(t->tag, "qexpr"))  { x = lval_qexpr(); }

  for (int i = 0; i < t->children_num; i++) {
    if (strcmp(t->children[i]->contents, "(") == 0) { continue; }
    if (strcmp(t->children[i]->contents, ")") == 0) { continue; }
    if (strcmp(t->children[i]->contents, "}") == 0) { continue; }
    if (strcmp(t->children[i]->contents, "{") == 0) { continue; }
    if (strcmp(t->children[i]->tag,  "regex") == 0) { continue; }
    x = lval_add(x, lval_read(t->children[i]));
  }

  return x;
}
// ############### END LVAL METHODS
