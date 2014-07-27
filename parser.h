#ifndef _PARSER_H_
#define _PARSER_H_
void generate_parsers(void);
void cleanup_parsers(void);
int parse(char *input);
void respond_to(int result_status);
#endif
