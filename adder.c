#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// cc -Wall -std=c99 adder.c -o adder

// now that I've extracted my debug fn from the hello_world.c
// I created:
// jt_debug.c
// and its header:
#include "jt_debug.h"
// to use the jt_debug.c here:
// cc -Wall -std=c99 adder.c jt_debug.c
//
// or to use jt_debug.o
// cc -Wall -std=c99 -o adder adder.c jt_debug.o
//
// or to use the SHARED (.so) or static (.a) library file:
// cc -Wall -std=c99 -o adder adder.c -L. -ljt_debug
// notice:
// (0) how we added the -L option for the current directory ...cuz our lib is not in the standard path
// (1) how we ignored the "lib" prefix (i.e. on libjt_debug.so)
// finally, to use link to the shared object (at runtime) make sure it's in the LD_LIBRARY_PATH
// export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH
// cc -Wall -std=c99 -o adder adder.c

char* string_add(char* x, char* y) {
  int number_of_chars_to_allocate = strlen(x) + strlen(y) + 1;
  char result[number_of_chars_to_allocate];
  char *result_p = &result[0];


  bzero(result_p,number_of_chars_to_allocate);
  //printf("debug: number_of_chars_to_allocate => %d\n", number_of_chars_to_allocate);
  debug("number_of_chars_to_allocate => %d\n", number_of_chars_to_allocate);
  //printf("debug: len(result) => %ld\n", strlen(result));
  debug("len(result) => %ld\n", strlen(result));

/*
  int index, result_index;
  for(int index=0; index<strlen(x); index++) {
    result_index = index;
    printf("debug: result[%d] => %c\n", result_index, x[index]);
    result[result_index] = x[index];
  }
  */
  strcat(result_p, x);
  //printf("debug: len(result) => %ld\n", strlen(result));
  debug("len(result) => %ld\n", strlen(result));

/*
  for(index=0; index<strlen(y); index++) {
    result_index = strlen(x) + index;
    printf("debug: result[%d] => %c\n", result_index, y[index]);
    result[result_index] = y[index];
  }
*/
  strcat(result_p, y);
  //printf("debug: len(result) => %ld\n", strlen(result));
  debug("len(result) => %ld\n", strlen(result));
  assert(result[number_of_chars_to_allocate] == '\0');

  return result_p;
}

double double_add(double x, double y) {
  return x + y;
}

int int_add(int x, int y) {
  return x + y;
}

int main(int argc, char** argv) {
  int int_x = 1;
  int int_y = 2;

  double double_x = 1.0001;
  double double_y = 2.0001;

  char* string_x = "abc";
  char* string_y = "def";
  //printf("debug: len(x) => %ld\n", strlen(string_x));

  int argument_count = argc - 1;

  if (argument_count >= 2) {
    int_x = atoi(argv[1]);
    int_y = atoi(argv[2]);
  }

  if (argument_count >= 4) {
    //double_x = strtod(argv[3], (char **)NULL);
    double_x = atof(argv[3]);
    //double_y = strtod(argv[4], (char **)NULL);
    double_y = atof(argv[4]);
  }

  if (argument_count >= 6) {
    string_x = argv[5];
    string_y = argv[6];
  }

  printf("integer sum of int_x(%d) and int_y(%d): %d\n", int_x, int_y, int_add(int_x, int_y));

  printf("double sum of double_x(%lf) and double_y(%lf): %lf\n", double_x, double_y, double_add(double_x, double_y));

  printf("string sum of string_x(%s) and string_y(%s): %s\n", string_x, string_y, string_add(string_x, string_y));
  return 0;
}
