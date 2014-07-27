#ifndef _PROMPT_H_
#define _PROMPT_H_
void welcome_message(void);
void prompt(void);
//void allocate_prompt(char *prompt_string);
char *generate_prompt(void);
int found(char *search_list[], char *key, int size);
void echo_message(char *message);
int found_an_exit_in(char *input_token);
void respond_to(char *parsed_input);
char *parse(char *input);
#endif
