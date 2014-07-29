#ifndef _PARSER_H_
#define _PARSER_H_
typedef struct lval {
  int type;

  long num;
  char* err;
  char* sym;

  int count;
  struct lval** cell;

} lval;

void generate_parsers(void);
void cleanup_parsers(void);
int parse(char *input);
void respond_to(int result_status);
char *grammar(int);
lval* lval_eval(lval* v);
void lval_print(lval* v);
void lval_println(lval* v);
lval* lval_add(lval* v, lval* x);
void lval_del(lval* v);
lval* lval_read(mpc_ast_t* t);
lval* lval_read_num(mpc_ast_t* t);
#define RPN 1
#define SIMPLE_SPEAK 2
#define JTLISP 3
#endif
