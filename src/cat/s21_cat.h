#ifndef TESTH
#define TESTH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct options {
  int b, e, n, s, t, v;
} options = {0};

void flag_parser(struct options *options, int argc, char **argv,
                 int *Error_code);
void add_flag(struct options *options, char *string_option, int *Error_code);
void cat_utility(int current, char *previous, struct options *options,
                 int *printed, int *index);
void print_file(int count_files, char **files, struct options *options);
void add_flag_long(struct options *options, char *string, int *Error);
void print_error();

#endif