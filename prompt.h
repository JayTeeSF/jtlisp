#ifndef _PROMPT_H_
#define _PROMPT_H_
void welcome_message(void);
char *prompt(void);
void allocate_prompt(char *prompt_string);
void echo_message(char *message);
void respond_to(char *parsed_input);
char *parse(char *input);
#endif
