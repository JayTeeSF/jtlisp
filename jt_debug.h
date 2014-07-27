#ifndef _JT_DEBUG_H_
#define _JT_DEBUG_H_
// my own version of fprintf (to stderr) that prepends "DEBUG:..."
void debug(const char* format_string, ...);
int number_of_arguments_in(int raw_argument_count);
int number_of_valid_inputs(int number_of_arguments, int number_of_arguments_for_a_valid_input);
#endif
