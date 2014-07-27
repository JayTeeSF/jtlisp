#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "jt_debug.h"

// cc -Wall -std=c99 -o grapher grapher.c jt_debug

typedef struct {
  float x;
  float y;
} point;

void display_point(point coordinate) {
  printf("(%0.2f,%0.2f)",coordinate.x, coordinate.y);
}

void display_points(point coordinates[], int count) {
  //int size_of_coordinates = sizeof(coordinates);
  //int number_of_points = size_of_coordinates / sizeof(point*);
  //debug("preparing to print %d points from a %d-sized-coord-array\n", number_of_points, size_of_coordinates);

  //for(int i=0; i < number_of_points; i++) {
  for(int i=0; i < count; i++) {
    debug("coordinate[%d]\n", i);
    //display_point(coordinates[i * sizeof(point*)]);
    if (i > 0) { printf("; "); }
    display_point(coordinates[i]);
  }
  printf("\n");
}

int main(int ac, char** av) {
  int number_of_arguments_for_a_valid_input = 2;
  int number_of_arguments = number_of_arguments_in(ac);
  int number_of_points = number_of_valid_inputs(number_of_arguments, number_of_arguments_for_a_valid_input);
  //point* locations = malloc(number_of_points * sizeof(*locations));
  //point* locations; // = malloc(number_of_points * sizeof(point*));
  //bzero(locations,(number_of_points * sizeof(point*)));

  point locations[number_of_points + 1]; // the extra 1 is just in case number_of_points is 0
  bzero(locations,(number_of_points * sizeof(point*)));

  //stuffing location(s) into an array (not quite sure why)
  debug("Out of %d arguments It seems the caller supplied %d valid points\n", number_of_arguments, number_of_points);
  for(int i=0; i < number_of_points; i++) {
    int first_argument_value_index = (number_of_arguments_for_a_valid_input * i) + 1;
    int second_argument_value_index = first_argument_value_index + 1;
    debug("using [%d].x => av[%d], [%d].y => av[%d]\n", i, first_argument_value_index, i, second_argument_value_index);
    //point newPoint = { atof(av[first_argument_value_index]), atof(av[second_argument_value_index]) };
    //point newPoint;
    //newPoint.x = atof(av[first_argument_value_index]);
    //newPoint.y = atof(av[second_argument_value_index]);
    //display_point(newPoint);
    //locations[i] = newPoint;

    // for some reason my array of locations forgets after each loop!
    locations[i].x = atof(av[first_argument_value_index]);
    locations[i].y = atof(av[second_argument_value_index]);
    //locations[i] = { atof(av[first_argument_value_index]), atof(av[second_argument_value_index]) };

    if (i > 0) { printf("| "); }
    display_point(locations[i]);
  }
  // default(s):
  if (0 >= number_of_points) {
    //point defaultPoint = {2.0, 3.0};
    //locations[0] = defaultPoint;
    debug("0 args\n");
    locations[0].x = 2.0;
    locations[0].y = 3.0;
    number_of_points = 1;
  } else {
    printf("\n\n");
  }

  debug("displaying points\n");
  display_points(locations, number_of_points);

  //free(locations);
  return 0;
}
